#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>


// Global variables

// Pins where the two potentiometers will be connected to
const int latPin = 32;
const int longPin = 33;
// Button pin
const int buttonPin = 5;
// Raw 12-bit values from the input pins that will be mapped later
int rawLatVal = 0;
int rawLongVal = 0;
// Variable to store the state of the button pin when using digitalRead()
int currButtonState;
int prevButtonState;


// Number of samples to take at a time
int samples = 128;
// numReadings is the number of readings over which the average is computed
const int numReadings = 6;
// Arrays to store the numReadings most recent readings and sum to keep track
int latReadings[numReadings];
int latSum;
int longReadings[numReadings];
int longSum;
// Index to keep track of the least recent value that has to be replaced
int idx = 0;
// The average values
float latAvg;
float longAvg;


// Create LiquidCrystal_I2C instance
LiquidCrystal_I2C lcd(0x27, 16, 2);
// "Degree" character
// Each LCD screen cell has 8 rows and 5 columns, so an array of type
// block is declared with 8 "bytes" of 5 bits each, all set to 1 to
// represent a solid block
byte degree[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};


// WiFi ssid and password for the esp32 to connect
const char* ssid = "wifi_ssid";
const char* password = "wifi_password";


// API token and url to send the request
const char* token = "iq_air_token";
String url;


// Character arrays to hold the latitude and longitide digits (plus decimal point)
char latBuffer[6];
char longBuffer[6];
String longSign;
// Weather info from the response of the http request
String weatherInfo = "";
// Index of the leftmost character to be displayed
int leftIdx = 0;
// Flag to scroll the text to the right or left
int forward = 1;
// Interval between updates (ms)
const long interval = 350;
// Declate variable to store the current time
long currentMillis;
// Previous update
long previousMillis = 0;


void setup() {
  // Begin Serial communication
  Serial.begin(115200);
  delay(1000);


  // Set the analog resolution
  analogReadResolution(11);

  Serial.println();
  Serial.println("Connecting to Wifi...");
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  // Once connected print the local IP address
  Serial.println();
  Serial.println("Connected to WiFi.");
  Serial.println("Local IP address:");
  Serial.println(WiFi.localIP());


  // Set buttonPin as INPUT with an internal pull-up
  pinMode(buttonPin, INPUT_PULLUP);
  // Set prevButtonState to HIGH assuming the button hasn't been pressed
  prevButtonState = HIGH;

 
  // Initialize the lcd screen
  lcd.init();
  lcd.backlight();
  // Create a the custom char and store it in pos 0 of CGRAM
  lcd.createChar(0, degree);
  // Display the character on columns 5 and 13
  lcd.setCursor(5, 0);
  lcd.write(byte(0));
  lcd.setCursor(14, 0);
  lcd.write(byte(0));


  // Fill the arrays with 0s initially
  for (int i = 0; i < numReadings; i++) {
    latReadings[i] = 0;
  }
  for (int i = 0; i < numReadings; i++) {
    longReadings[i] = 0;
  }
  // Initially the sums are 0
  latSum = 0;
  longSum = 0;
}


void loop() {
  // Read analog values from latPin and longPin with multisampling
  rawLatVal = readAnalogMultisampled(latPin, samples);
  rawLongVal = readAnalogMultisampled(longPin, samples);


  // Update the array and the current average values
  movingAverage(rawLatVal, rawLongVal);

  // Display latitude and longitude values on the lcd screen
  displayLatLong();

  // Check if the button has been pressed
  currButtonState = digitalRead(buttonPin);
  if (currButtonState == LOW && prevButtonState == HIGH) {
    // If the button has been pressed, then send a request to get 
    Serial.println("Pressed button!");
    String strLatBuffer = String(latBuffer);
    String strLongBuffer = String(longBuffer);
    strLatBuffer.trim();
    strLongBuffer.trim();
    url = "http://api.airvisual.com/v2/nearest_city?lat=" + strLatBuffer + "&lon=" + longSign + strLongBuffer + "&key=" + String(token);
    weatherInfo = getWeatherInfo(url);
    forward = 1;
    leftIdx = 0;
  }
  
  // Serial.print(latBuffer);
  // Serial.print(" ");
  // Serial.println(longBuffer);
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    displayWeatherInfo(weatherInfo);
    previousMillis = currentMillis;
  }


  // Store the previous button state
  prevButtonState = currButtonState;

  delay(100);
}


// Function to convert raw 12-bit values to latitude
float mapToLat(int raw) {
  // Latitude value range for Europe is 34 deg to 71 deg
  float latVal = ((71.0 - 34.0) / 2047.0) * raw + 34.0;
  return latVal;
}


// Function to convert raw 12-bit values to longitude
float mapToLong(int raw) {
  // Latitude value range for Europe is -10 deg to 60 deg
  float longVal = ((60.0 - (-10.0)) / 2047.0) * raw - 10.0;
  return longVal;
}


// Perform multisampling to decrease noise
int readAnalogMultisampled(int pin, int samples) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
  }
  // Divide by the number of samples (bit shift to right for power of 2)
  return sum >> (int)log2(samples);
}


// Use a simple moving average filter to further decrease noise
void movingAverage(int newRawLatVal, int newRawLongVal) {
  // Subtract the least recent value
  latSum -= latReadings[idx];
  longSum -= longReadings[idx];
  // Add the new value to the array
  latReadings[idx] = newRawLatVal;
  longReadings[idx] = newRawLongVal;
  // Add the new value
  latSum += newRawLatVal;
  longSum += newRawLongVal;
  // Go to the next index
  idx = (idx + 1) % numReadings;
  // Compute the mean values
  latAvg = latSum / numReadings;
  longAvg = longSum / numReadings;
}


// Function to display the latitude and longitude on first row
void displayLatLong() {
  // Convert the 11-bit values to latitude and longitude in degrees
  float latDeg = mapToLat(latAvg);
  float longDeg = mapToLong(longAvg);
  // Read the digits to the character buffers to display them on the lcd
  dtostrf(latDeg, 5, 2, latBuffer);
  dtostrf(abs(longDeg), 5, 2, longBuffer);
  // Display the values
  lcd.setCursor(0, 0);
  lcd.print(latBuffer);
  lcd.setCursor(6, 0);
  lcd.print("N"); // North
  lcd.setCursor(9, 0);
  lcd.print(longBuffer);
  lcd.setCursor(15, 0);
  if (longDeg < 0) { // If less than 0 then print W for West
    lcd.print("W");
    longSign = "-";
  }
  else { // Else E for East
    lcd.print("E");
    longSign = "";
  }
}


// Function to send the request and store the response to be displayed in a string
String getWeatherInfo(String url) {
  // Declare string to store the text to display
  String displayText;


  HTTPClient http;
  // Begin the http request
  http.begin(url);
  // Send the GET request
  int httpResponseCode = http.GET();


  // Check the returning code
  if (httpResponseCode > 0) {
    String response = http.getString();
    displayText = deserialize(response);
  }
  else {
    displayText = "Error code " + String(httpResponseCode);
  }


  return displayText;
}


// Function to deserialize the json string to access the contents easily
String deserialize(String json) {
  // Declare string to store the text to display
  String displayText;


  // JsonDocument instance
  JsonDocument doc;
  // Deserialize the json string
  deserializeJson(doc, json);


  // The conents can be accessed by keys like in python dictionaries
  if (doc["status"] == "success") {
    // Weather codes
    String weatherIndex = doc["data"]["current"]["weather"]["ic"];
    String weatherDesc = "";
    if (weatherIndex == "01d") {
      weatherDesc = "Clear sky day-time";
    }
    else if (weatherIndex == "01n") {
      weatherDesc = "Clear sky night-time";
    }
    else if (weatherIndex == "02d") {
      weatherDesc = "Few clouds day-time";
    }
    else if (weatherIndex == "02n") {
      weatherDesc = "Few clouds night-time";
    }
    else if (weatherIndex == "03d") {
      weatherDesc = "Scattered clouds";
    }
    else if (weatherIndex == "04d") {
      weatherDesc = "Broken clouds";
    }
    else if (weatherIndex == "09d") {
      weatherDesc = "Shower rain";
    }
    else if (weatherIndex == "10d") {
      weatherDesc = "Rain day-time";
    }
    else if (weatherIndex == "10n") {
      weatherDesc = "Rain night-time";
    }
    else if (weatherIndex == "11d") {
      weatherDesc = "Thunderstorm";
    }
    else if (weatherIndex == "13d") {
      weatherDesc = "Snow";
    }
    else if (weatherIndex == "50d") {
      weatherDesc = "Mist";
    }
    displayText = String(doc["data"]["city"]) + "," +
                  String(doc["data"]["country"]) + 
                  " " + weatherDesc + 
                  " tp=" + String(doc["data"]["current"]["weather"]["tp"]) + 
                  " pr=" + String(doc["data"]["current"]["weather"]["pr"]) + 
                  " hu=" + String(doc["data"]["current"]["weather"]["hu"]) +
                  " aqi=" + String(doc["data"]["current"]["pollution"]["aqius"]) + " ";
  }
  else {
    Serial.println(json);
    displayText = "No data is available!";
  }

  
  return displayText;
}

// Function to display the weather information as scrolling text on bottom row
void displayWeatherInfo(String info) {
  // Serial.println(info);
  lcd.setCursor(0, 1);
  lcd.print(info.substring(leftIdx,leftIdx+16));
  if (forward == 1) {
    leftIdx++;
  }
  else {
    leftIdx--;
  }

  if (leftIdx == 0 or leftIdx == info.length() - 16) {
    forward = 1 - forward;
  }
}
