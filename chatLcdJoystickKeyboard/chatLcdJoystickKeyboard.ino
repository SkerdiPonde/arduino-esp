#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

// Screen to display the typed text
LiquidCrystal_I2C lcdtext(0x27, 16, 2); 
// Screen to display characters and symbols -> keyboard
LiquidCrystal_I2C lcdkey(0x23, 16, 2);

// Create a character arrays to hold the lowercase and uppercase
// letters respectively
// 0th "page" will store the lowercase letters
// 1st "page" will store the uppercase letters
char keyboard[2][2][16];

// Store the current keyboard configuration uppercase or lowercase
int keyboardType = 0;

// Cursor position on keyboard lcd screen
int row = 0;
int col = 0;

// Cursor postion on text lcd screen
int rowText = 0;
int colText = 5;

// Cursor position change interval and last time it changed in ms
const unsigned long cursorInterval = 150;
unsigned long lastChange = 0;
// Store current time in ms
unsigned long currentMillis;

// Store the text that's written
String text = "";

// Custom character to denote the change of letters to uppercase
byte upArrow[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};
// Custom character to denote the change of letters to lowercase
byte downArrow[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000
};
// Custom character for the backspace
byte backspace[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01000,
  0b11111,
  0b01000,
  0b00100,
  0b00000
};
// Custor character for enter
byte enter[8] = {
  0b00100,
  0b01000,
  0b11111,
  0b01001,
  0b00101,
  0b00001,
  0b00001,
  0b00000
};


// Joystick pins
const int buttonPin = 5;
const int xPin = 32;
const int yPin = 33;

// Store the joystick button state
bool buttonState = HIGH;
bool lastButtonState = HIGH;

// Joystick button debounce timer
const unsigned long debounceDelay = 40;
unsigned long lastDebounceTime = 0;

// Check if joystick button has been pressed
bool buttonPressed = false;

// Wifi ssid and passkey
const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

const char* token = "huggingface_token";

// const char* repo_id = "mistralai/Mistral-7B-Instruct-v0.3";
const char* API_URL = "https://api-inference.huggingface.co/models/mistralai/Mistral-7B-Instruct-v0.3";

// Global variables to store user input and bot response
String userInput = "User:Provide short and concise answers even when prompted to write something longer. ";
String botResponse = "Sure. User:";
String displayBotResponse = "";
int responseLength = 0;

// Variables related to the bot's response
// True if bot's response is being shown on screen
bool displayBot = false;
const unsigned long duration = 150; // wait before showing next char
unsigned long lastCharacterTime = 0; // the time the previous one was displayed
String buffer = ""; // store the string in row 1 
int idx = 0;  // idx of the current character being shown

void setup() {
  // Begin serial communication
  Serial.begin(115200);

  // Initialize the lcd screens
  lcdtext.init();
  lcdtext.backlight();
  lcdkey.init();
  lcdkey.backlight();
  
  // Create the custom characters
  lcdkey.createChar(0, upArrow);
  lcdkey.createChar(1, downArrow);
  lcdkey.createChar(2, backspace);
  lcdkey.createChar(3, enter);

  // Fill the arrays with values
  fillKeyboardLower();
  fillKeyboardUpper();

  // Set analog resolution
  analogReadResolution(10);
 
  // Initialize joystick's switch pin as an input with a pull up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  lcdkey.setCursor(col, row);
  // Make the cursor blink on the keyboard lcd
  lcdkey.blink();

  lcdtext.print("User:");
  lcdtext.setCursor(colText, rowText);
  // Make the cursor blink on the keyboard lcd
  lcdtext.blink();

  // Initially place the lowercase keyboard on screen
  updateKeyScreen(keyboardType);

  // Connect to WiFi
  Serial.println();
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // While the connection to WiFi hasn't been established
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Confirm the WiFi connection by printing the local IP
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // Call function to place the cursor on the correct position based
  // on the joystick input
  placeCursor();

  // Call function to check if button has been pressed
  checkButton();

  // If the bot's response is to be displayed and it's last character hasn't been shown
  if (displayBot && idx < responseLength) {
    int currTime = millis();
    // Display a character every duration ms
    if (currTime - lastCharacterTime >= duration) {
      lastCharacterTime = currTime;
      displayResponseOnScreen(responseLength);
    }
  }
  else {
    // A button press is determined by the previous function
    if (buttonPressed) {
      // Call the function that manages the text string saved internally
      joyButtonEvent(keyboardType);
      // Update the text  on the lcd screen in case a character is added or removed
      updateTextScreen(keyboardType);
    }
  }
  
  // Reset the state
  buttonPressed = false;
}


// Functions to fill the character arrays with values
// Fill for lowercase
void fillKeyboardLower() {
  char row0[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
  char row1[16] = {'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', ',', ' ', char(2), char(3), char(0)};
  for (int i = 0; i < 16; i++) {
    keyboard[0][0][i] = row0[i];
    keyboard[0][1][i] = row1[i];
  }
}

// Fill for uppercase
void fillKeyboardUpper() {
  char row0[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
  char row1[16] = {'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '?', '!', ' ', char(2), char(3), char(1)};
  for (int i = 0; i < 16; i++) {
    keyboard[1][0][i] = row0[i];
    keyboard[1][1][i] = row1[i];
  }
}

// Function to change the position of the cursor based on the input provided 
// by the joystick
void placeCursor() {
  currentMillis = millis();
  // Check if it's time to update
  if (currentMillis - lastChange >= cursorInterval) {
    // Read the analog values from the joystick's x and y pins
    int xVal = analogRead(xPin);
    int yVal = analogRead(yPin);
    // Update cursor x position making sure it is within bounds
    // Loop around for y direction (horizontal in this case)
    if (yVal >= 768) { // right
      col = (col + 1) % 16;
    }
    else if (yVal <= 256) { // left
      col = (((col - 1) % 16) + 16) % 16;
    }
    // Update cursor y position making sure it is within bounds
    if (xVal >= 768) { // down
      row = min(1, row + 1); 
    }
    else if (xVal <= 256) { // up
      row = max(0, row - 1);
    }
    lcdkey.setCursor(col, row);
    lastChange = currentMillis;
  }
}

// Function to check the state of the joystick button/switch
void checkButton() {
  // Check if the debounce time has passed
  if ((millis() - lastDebounceTime) >= debounceDelay) {
    // Read the state of the button
    int reading = digitalRead(buttonPin);
    // Check if the button state has changed
    if (reading != lastButtonState) {
      // Reset the debounce timer
      lastDebounceTime = millis();
      // If the button state has changed
      if (reading == LOW) {
        lastButtonState = LOW;
        buttonPressed = true;
      }
      else {
        lastButtonState = HIGH;
      }
    } 
  }
}

// Function to update the string text
void joyButtonEvent(int keyType) {\
  // If the bot's response is being displayed only register Enter presses
  if (displayBot) {
    // When enter is pressed reset the buffer and set displayBot to false
    if (row == 1 && col == 14) {
      buffer = "";
      displayBot = false;
    }
  }
  else {
    // If the text lcd screen is full don't do anything to the string
    if (rowText == 1 && colText > 15) {
      if (row == 1 && col == 14) {
        displayBot = true;
        prompt(text);
        // Reset the string
        text = "";
        idx = 0;    
      }
      return;
    }
    // Backspace is pressed
    if (text && row == 1 && col == 13) {
      text.remove(text.length()-1);
    }
    // Enter is pressed
    else if (row == 1 && col == 14) {
      displayBot = true;
      prompt(text);
      // Reset the string
      text = "";
      idx = 0;    
    }
    // Switch to upper/lowercase
    else if (row == 1 && col == 15) {
      keyboardType = 1 - keyboardType;
      updateKeyScreen(keyboardType);
    }
    // A character is added to the string
    else {
      text += keyboard[keyType][row][col];
    }
    // Print the current string in serial
    Serial.println(text);
  }
}


// Update the text lcd screen
void updateTextScreen(int keyType) {
  // If the bot's response is being displayed only register Enter presses
  if (displayBot) {
    // When enter is pressed print "Bot:" on screen and place the cursor at the next pos
    if (row == 1 && col == 14) {
      lcdtext.clear();
      lcdtext.print("Bot:");
      rowText = 0;
      colText = 4;
      lcdtext.setCursor(colText, rowText);
    }
  }
  else {
    // If the keyboard change key is pressed or if the screen is full
    if (row == 1 && col == 15) {
      return;
    }
    // Enter is pressed and the users text is to be displayed
    // Print "User:" on screen and set cursor to next index
    if (row == 1 && col == 14) {
      lcdtext.clear();
      lcdtext.print("User:");
      rowText = 0;
      colText = 5;
      lcdtext.setCursor(colText, rowText);  
    }
    // Backspace is pressed
    else if (row == 1 && col == 13) {
      colText--;
      // Handle case when the text cursor is in 5th col of row 0
      // After "User:"
      if (rowText ==  0 && colText == 4) {
        colText = 5;
      }
      // Handle case when the cursor has moved to row 1
      else if (rowText == 1 && colText == -1) {
        rowText = 0;
        colText = 15;
      }
      lcdtext.setCursor(colText, rowText);
      // Replace last character with space and move the cursor
      lcdtext.print(" ");
      lcdtext.setCursor(colText, rowText);
    }
    // A character is added
    else {
      // Print new character
      lcdtext.print(keyboard[keyType][row][col]);
      colText++;
      // Handle case when cursor is in 15th col of row 0
      if (rowText ==  0 && colText == 16) {
        rowText = 1;
        colText = 0;
      }
      // Handle case when screen if full
      else if (rowText ==  1 && colText == 17) {
        colText -= 1; // colText doesn't change
      }
      // Move cursor to the next position
      lcdtext.setCursor(colText, rowText);
    }
  }
}


// Function to update the keyboard screen to lower/uppercase version
void updateKeyScreen(int keyType) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 16; j++) {
      lcdkey.setCursor(j, i);
      lcdkey.write(keyboard[keyType][i][j]);
    }
  }
}


// Function to send the request to the api and receive bot's response
String sendMessage(const char* apiUrl, const char* token, String userInput) {
  // Use ArduinoJson library to create the payload
  JsonDocument payloadJson;
  payloadJson["inputs"] = userInput + "Bot:";
  JsonObject parameters = payloadJson.createNestedObject("parameters");
  parameters["max_new_tokens"] = 3000;
  parameters["stop"][0] = "User:";
  parameters["stop"][1] = "Bot:";
  // parameters["stop"] = {"User:", "Bot:"};

  // Convert it from JsonDocument to String
  String payload;
  serializeJson(payloadJson, payload);

  // Declare an HTTPClient instance and initialize it
  HTTPClient http;
  http.begin(apiUrl);
  http.addHeader("Authorization", "Bearer " + String(token));
  http.addHeader("Content-Type", "application/json");

  // Send a HTTP POST request and store the response code
  int httpResponseCode = http.POST(payload);

  // String to store the bot's answer
  String answer;

  // If the response code is larger than 0, meaning a successful request
  if (httpResponseCode > 0) {
    // Get the response as a string
    String response = http.getString();
    // response = response.substring(1, len(response)-1);
    Serial.println(response);

    // Convert it to a JsonDocument
    JsonDocument responseJson;
    deserializeJson(responseJson, response);

    // The response is a JSON array, get the first element
    // The bot's answer can be accessed by the "generated_text" keyword
    answer = String(responseJson.as<JsonArray>()[0]["generated_text"]);
    answer.trim();
    answer = answer.substring(answer.lastIndexOf("Bot:") + 4);

  }
  // Otherwise assign a message that contains the response code to answer
  else {   
    answer = "Error:" + String(httpResponseCode); 
  }
  // Serial.println(answer);

  // End http connection and return the answer
  http.end();
  return answer;
}


// Function to handle the new user input and the answer from the bot
void prompt(String newUserInput) {
  newUserInput.trim(); // Remove surrounding spaces

  // Append the user input with the bot's response + the new user input
  userInput += "Bot:" + botResponse + newUserInput + " ";

  // Call senMessage to make the api request
  botResponse = sendMessage(API_URL, token, userInput);
  
  // Sanitize the response string by removing spaces and the "User:" suffix for display
  displayBotResponse = botResponse.substring(0, botResponse.lastIndexOf("User:"));
  displayBotResponse.trim();
  // displayBotResponse.replace("\n", " ");
  responseLength = displayBotResponse.length();
  Serial.println(displayBotResponse);
}


// Function to display one character at a time from the response
void displayResponseOnScreen(int length) {
  colText++;
  if (displayBotResponse[idx] != '\n') {
    lcdtext.print(displayBotResponse[idx]);
    // Handle case when cursor is in 15th col of row 0
    if (rowText ==  0 && colText == 16) {
        rowText = 1;
        colText = 0;
      }
    // Only append buffer with characters of first row
    else if (rowText ==  1) {
        buffer += displayBotResponse[idx];
      // Handle case when screen if full
      if (colText == 16) {
        // "Move" the part at row 1 to row 0
        lcdtext.clear();
        lcdtext.setCursor(0, 0);
        lcdtext.print(buffer);
        rowText = 1;
        colText = 0;
        // Reset buffer
        buffer = "";
      }
    }
  }
  // If a newline is present move to the next row and reset buffer
  else {
    lcdtext.clear();
    lcdtext.setCursor(0, 0);
    lcdtext.print(buffer);
    rowText = 1;
    colText = 0;
    buffer = "";
  }
  lcdtext.setCursor(colText, rowText);
  // Move to the next character
  idx++;
}