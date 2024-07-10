# Arduino and ESP32 Projects

This repository contains various projects implemented using Arduino Uno (ATmega328P) and ESP32 microcontrollers. The projects include applications such as distance sensors, LCD displays, and interfacing with APIs for natural language processing tasks.

Explore each project folder for detailed descriptions and code implementations specific to each project.

## Projects included so far

- **Chat Bot Interface with Hugging Face API**: Implements ESP32 to interface with Hugging Face API for natural language processing tasks, displaying responses on an LCD screen.
- **Distance Sensor with Buzzer and LCD Display**: Utilizes Arduino Uno and HC-SR04 ultrasonic sensor for distance measurement with auditory feedback and LCD display.
- **Volume Display with Potentiometer and LCD Screen**: Displays a volume bar on a 16x2 LCD screen using Arduino Uno and a potentiometer, visualizing analog input as blocks on the screen.
- **Weather Information Display with ESP32 and LCD**: Uses an ESP32 to select latitude and longitude values within Europe, display them on a 16x2 I2C LCD screen, and retrieve weather and air quality information for the nearest city from the AQI Air API.
- **Joystick-Based LCD Keyboard with Arduino**: Explores the usage of a joystick with an LCD screen to act as a keyboard, allowing for switching between lowercase and uppercase letters and special characters.
- **Joystick-Controlled Text Input with ESP32 and Hugging Face API**: Combines the concepts of a joystick-based virtual keyboard and the chatbot, allowing users to navigate and select characters on one screen, display the typed text on another, and send the text to a chatbot model on Hugging Face's platform to display the response.

Each project folder includes circuit diagrams (Fritzing), Arduino sketches (.ino), and desc.md files detailing setup, components used, and operation instructions.

Additionally, the repository includes smaller projects and tests inside the **Other_tinkering** folder.

Feel free to explore and use these projects for learning and experimentation with Arduino and ESP32 microcontrollers.
