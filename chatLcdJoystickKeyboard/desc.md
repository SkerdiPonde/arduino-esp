# Joystick-Controlled Text Input with ESP32 and Hugging Face API

## Author: Skerdi Ponde

This project combines the concepts of two previous projects, **lcdJoystickKeyboard** and **lcdMistral**, into a single system. It explores using a joystick with two LCD screens to create a chatbot with text input system. The joystick navigates and selects characters on a virtual keyboard displayed on one screen, while the typed text is displayed on another. The system can send the typed text to a chatbot model hosted on Hugging Face's platform and display the response.

## Components Used

- **DOIT ESP32 DEVKIT v1**
- **Joystick Module**
- **16x2 I2C LCD Screen (x2)**
- **Breadboard and Jumper Wires**

## Fritzing Diagram

In the Fritzing diagram, the ESP32 component was downloaded from this [ESP32 Fritzing Part](https://forum.fritzing.org/uploads/short-url/fXKlUH6QzPM1DE8UeskUInu5yaj.fzpz) by [vanepp](https://forum.fritzing.org/u/vanepp/summary). 

The LCD screen I2C component was downloaded from this [link](https://forum.fritzing.org/uploads/default/original/2X/7/70f90addd7883759e4a0d06a934946c2be8aa6c1.fzpz).

## How It Works

1. **Initialization**: The ESP32 initializes the two LCD screens and connects to the specified Wi-Fi network.
2. **Joystick Navigation**: The joystick is used to navigate the virtual keyboard displayed on the first LCD screen. The joystick's x and y axes move the cursor, and the switch selects the current character.
3. **Debouncing**: Software debouncing is implemented to handle the mechanical bouncing of the joystick switch.
4. **Keyboard Input**: The keyboard includes lowercase and uppercase letters, punctuation marks, and special characters such as backspace and enter.
5. **Display Text**: The typed text is displayed on the second LCD screen.
6. **Chatbot Interaction**: The user's input is sent to a chatbot hosted on Hugging Face's platform, and the response is displayed on the text LCD screen.
7. **Context Retention**: Every new user input and bot response is appended to the input/payload string to maintain the conversation context over multiple interactions.


## Usage

1. **Connect Components**: Assemble the components as per the Fritzing diagram.
2. **Upload Code**: Upload the provided code to your ESP32.
3. **Monitor Serial**: Open the Serial Monitor in your Arduino IDE to see potential debug messages.
4. **Navigate Keyboard**: Use the joystick to navigate the keyboard and press the joystick switch to select characters.
5. **Switch Case**: Use the designated key on the keyboard to switch between lowercase and uppercase letters.
6. **Send Text to Chatbot**: Press the enter key to send the typed text to the chatbot. The response will be displayed on the text screen.
7. **Wait for Response**: Wait until the chatbot response is displayed in its entirety on the text screen. Once the full response is displayed, you can press the enter key to type another message.

## Acknowledgments

- **LiquidCrystal_I2C Library**: Used for interfacing with the I2C LCD screens.
- **ArduinoJson Library**: For handling JSON data.
- **WiFi and HTTPClient Libraries**: For enabling Wi-Fi connectivity and HTTP requests on the ESP32.
- **Hugging Face**: For providing accessible machine learning models through their API.
- **Fritzing Parts**: ESP32 component from the provided link, and the LCD screen I2C component from the previous link.