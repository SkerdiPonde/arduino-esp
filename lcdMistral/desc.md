# ESP32 Chat Bot with Hugging Face API and LCD Display

## Author: Skerdi Ponde

This project uses an ESP32 (DOIT ESP32 Devkit v1) to interact with a Hugging Face API model (Mistral-7B Instruct v0.3) and display the bot's response on a 16x2 I2C LCD screen. The ESP32 reads user input via serial, sends it to the API, and displays the response on the LCD.

## Components Used

- **ESP32 (DOIT ESP32 Devkit v1)**
- **16x2 I2C LCD Screen** (LiquidCrystal_I2C library)
- **Jumper Wires**

## Fritzing Diagram

In the Fritzing diagram, the ESP32 component was downloaded from the following link:
- [ESP32 Fritzing Part](https://forum.fritzing.org/uploads/short-url/fXKlUH6QzPM1DE8UeskUInu5yaj.fzpz) by [vanepp](https://forum.fritzing.org/u/vanepp/summary).

For the I2C module, the **LCM1602 IIC** model was used, and the part was downloaded from this [link](https://fritzing.org/projects/lcd-i2c-analog-inputoutput-displayed).

## How It Works

1. **Initialization**: The ESP32 connects to a WiFi network using the provided SSID and password.
2. **User Input**: The user sends a message through the serial monitor.
3. **API Communication**: The ESP32 sends the user input to the Hugging Face API using the Mistral-7B Instruct model. The API processes the input and returns a response.
4. **Display Response**: The bot's response is displayed on the 16x2 I2C LCD screen.

## Usage

1. **Connect Components**: Assemble the components as per the Fritzing diagram.
    - **Note**: The LCD VCC is connected to Vin since it requires 5V. Vin is also the voltage of the supply (Desktop USB to Type-C) to the ESP32. If this supply voltage is higher, it might damage the LCD screen.
2. **Upload Code**: Upload the provided `.ino` file to your ESP32.
3. **Monitor Serial**: Open the Serial Monitor in your Arduino IDE.
4. **Send Message**: Type your message in the Serial Monitor and press Enter.
5. **View Response**: The bot's response will be displayed on the LCD screen.

## Hugging Face API

This project uses the Mistral-7B Instruct model from Hugging Face. More information about the model can be found [here](https://huggingface.co/mistralai/Mistral-7B-v0.1).

## Acknowledgments

- **LiquidCrystal_I2C Library**: Used for interfacing with the I2C LCD screen.
- **Hugging Face API**: For providing the Mistral-7B Instruct model.
- **ESP32 Fritzing Part**: Provided by [vanepp](https://forum.fritzing.org/u/vanepp/summary) on the Fritzing forum.
- **I2C Module (LCM1602 IIC)**: Part downloaded from this [link](https://fritzing.org/projects/lcd-i2c-analog-inputoutput-displayed).