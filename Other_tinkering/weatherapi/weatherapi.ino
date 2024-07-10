#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

const char* token = "iq_air_api_token";

const String url = "http://api.airvisual.com/v2/nearest_city?key=" + String(token);

void setup() {
  // Begin Serial communication
  Serial.begin(115200);
  delay(10);

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


  // Call the function to send a request to url
  sendRequest(url);
}

void loop() {
  // Do nothing for now
}

void sendRequest(const String api_url) {
  // String payload = "";
  
  HTTPClient http;

  // Begin the HTTP request
  http.begin(api_url);

  // Send the GET request
  int httpResponseCode = http.GET();
  
  // Check the returning code
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);
    Serial.println(response);

    // Parse JSON
    const size_t capacity = JSON_OBJECT_SIZE(3) + 400; // Adjust capacity
    DynamicJsonDocument doc(capacity);

    // Deserialize JSON
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      // Close connection
      http.end();
      // Return
      return;
    }

    // Accessing elements
    const char* city = doc["data"]["city"];
    int aqius = doc["data"]["current"]["pollution"]["aqius"];

    // Print data
    Serial.print("City: ");
    Serial.println(city);
    Serial.print("AQI (US): ");
    Serial.println(aqius);
  }
  else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpResponseCode);
  }

  // Close connection
  http.end();

}
