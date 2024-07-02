# Weather Information Display with ESP32 and LCD

## Author: Skerdi Ponde

This project uses an ESP32 to select latitude and longitude values within Europe, display them on a 16x2 I2C LCD screen, and retrieve weather and air quality information for the nearest city from the AQI Air API.

## Components Used

- **DOIT ESP32 DEVKIT v1**
- **Push Button**
- **2 WH148 Rotary Potentiometers**
- **2 Ceramic Capacitors (104)**
- **16x2 I2C LCD Screen** (LiquidCrystal_I2C library)
- **Breadboard and Jumper Wires**

## Fritzing Diagram

In the Fritzing diagram, the ESP32 component was downloaded from the following link:
- [ESP32 Fritzing Part](https://forum.fritzing.org/uploads/short-url/fXKlUH6QzPM1DE8UeskUInu5yaj.fzpz) by [vanepp](https://forum.fritzing.org/u/vanepp/summary).

The LCD screen I2C component was downloaded from this [link](https://forum.fritzing.org/uploads/default/original/2X/7/70f90addd7883759e4a0d06a934946c2be8aa6c1.fzpz).

## How It Works

1. **Initialization**: The ESP32 connects to WiFi and initializes the LCD screen.
2. **User Input**: Two potentiometers are used to select latitude and longitude values. Multi-sampling and a moving average filter (in addition to the capacitors) are applied to reduce noise.
3. **Display Coordinates**: The latitude and longitude values are displayed on the first row of the LCD screen.
4. **Button Press**: A push button is used to trigger an HTTP GET request to the AQI Air API using the selected coordinates.
5. **Retrieve Data**: Weather and air quality data for the nearest city are retrieved and parsed from the API response.
6. **Display Weather Info**: The weather information is displayed as scrolling text on the second row of the LCD screen.

## Usage

1. **Connect Components**: Assemble the components as per the Fritzing diagram.
2. **Upload Code**: Upload the provided `.ino` file to your ESP32.
3. **Monitor Serial**: Open the Serial Monitor in your Arduino IDE to see potential debug messages.
4. **Adjust Potentiometers**: Rotate the potentiometers to change the latitude and longitude values displayed on the LCD screen.
5. **Press Button**: Press the push button to retrieve and display weather information for the nearest city.

## Acknowledgments

- **LiquidCrystal_I2C Library**: Used for interfacing with the I2C LCD screen.
- **ArduinoJson Library**: Used for parsing JSON data from the API response.
- **Fritzing Parts**: ESP32 model and LCD screen I2C from the provided links.