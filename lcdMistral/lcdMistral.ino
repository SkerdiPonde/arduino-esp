#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

const char* token = "huggingface_token";

// Mistral-7B inference api url
// Website link "https://huggingface.co/mistralai/Mistral-7B-v0.1"
const char* API_URL = "https://api-inference.huggingface.co/models/mistralai/Mistral-7B-Instruct-v0.3";

// Global variables to store user input and bot response
// Initial prompt to the chat bot
String user_input = "User: Provide short and concise answers even when prompted to write something longer. ";
String bot_response = " Sure. User: ";
String displayed_bot_response;

// Create LiquidCrystal_I2C instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(10);

  // Initialize the LCD screen
  lcd.init();
  lcd.backlight();
  delay(1000);

  // Place cursor on first col (0) of first row (0)
  lcd.setCursor(0, 0);
  lcd.print("Bot:");

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
  while (Serial.available() <= 0) {
    delay(100);
  }

  // Read input string from serial
  String new_user_input = Serial.readStringUntil('\n');
  new_user_input.trim(); // Remove surrounding spaces

  // Append the user input with the bot's response + the new user input
  user_input += "Bot: " + bot_response + " " + new_user_input + " ";

  // Ensure no unwanted control characters are in the user_input
  user_input.replace("\n", " ");
  user_input.replace("\r", " ");

  // Get the bot response from the api 
  bot_response = send_message(API_URL, token, user_input);
  // The bot response will contain an extra "User:" string at the end, because the bot
  // is not actually responding, but appending the input string while trying to imitate
  // the pattern of the input
  displayed_bot_response = bot_response.substring(0, bot_response.lastIndexOf("User:"));
  // Handling extra spaces or newlines
  displayed_bot_response.trim();
  displayed_bot_response.replace("\n", " ");
  displayed_bot_response.replace("\\n", " ");
  displayed_bot_response.replace("\\\"", "\"");

  // Also print the user's message and bot's response on serial 
  Serial.print("User: ");
  Serial.println(new_user_input);
  Serial.print("Bot: ");
  Serial.println(displayed_bot_response);

  // Display bot's response on LCD screen
  displayText(displayed_bot_response);

}

// Function to send the request to the api and receive bot's response
String send_message(const char* api_url, const char* token, String user_input) {
  // Create the payload
  String payload1 = "{\"inputs\": \"";
  String payload2 = user_input;
  String payload3 = "Bot: \", \"parameters\": {\"max_new_tokens\": 30000, \"stop\": [\"User:\", \"Bot:\"]}}";
  String payload = payload1 + payload2 + payload3;
  
  // Declare an HTTPClient instance and initialize it
  HTTPClient http;
  http.begin(api_url);
  http.addHeader("Authorization", "Bearer " + String(token));
  http.addHeader("Content-Type", "application/json");

  // Send a HTTP POST request and store the response code
  int httpResponseCode = http.POST(payload);

  String bot_response = "";

  // If the response code is larger than 0, meaning a successful request
  if (httpResponseCode > 0) {
    // Get the json string
    String response = http.getString();
    Serial.println(response);

    // Extract the bot's response from the API response by going 
    // The bots response comes after the keyword generated_text 
    // Find the starting and ending index of the response
    int startIndex = response.indexOf("generated_text\":\"") + 17;
    int endIndex = response.indexOf("}", startIndex) - 1;

    // Serial.print(startIndex);
    // Serial.println(endIndex);

    // If they have been found store the new bot response
    // The new bot's response is what comes after "Bot:"
    if (startIndex != -1 && endIndex != -1) {
      bot_response = response.substring(startIndex, endIndex);
      bot_response = bot_response.substring(bot_response.lastIndexOf("Bot:") + 4);
    }
    // Note: A better way to deal with json strings would be to use "ArduinoJson.h"
    // for deserialization. Each section can be accessed by using keys, e.g. "generated_text".
  }
  // Otherwise print the error code on the serial monitor
  else {
    Serial.print("Error: ");
    Serial.println(httpResponseCode);
  }

  // End http connection
  http.end();
  // Return the bot response
  return bot_response;
}


void displayText(String text) {
  lcd.setCursor(0, 1); // Set cursor on second row
  lcd.print("                "); // Clear the second row

  // Display the text on second row, handling scrolling if necessary
  if (text.length() > 16) {
    for (int i = 0; i <= text.length() - 16; i+=1) {
      lcd.setCursor(0, 1);
      lcd.print(text.substring(i, i + 16));
      delay(550);
    }
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print(text);
  }
}