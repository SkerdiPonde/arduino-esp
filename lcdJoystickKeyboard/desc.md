# Joystick-Based LCD Keyboard with Arduino

## Author: Skerdi Ponde

This project explores the usage of a joystick with an LCD screen to act as a keyboard. Key presses are detected through the joystick switch, which requires debouncing due to mechanical bouncing. The keyboard allows for switching between lowercase and uppercase letters, changing some characters (e.g., '.' to '?', ',' to '!'). The typed text is displayed on another LCD screen.

## Components Used

- **Arduino Uno (ATmega328P)**
- **Joystick Module**
- **16x2 I2C LCD Screen (x2)**
- **Breadboard and Jumper Wires**

## Fritzing Diagram

In the Fritzing diagram, the LCD screen I2C component was downloaded from this [link](https://forum.fritzing.org/uploads/default/original/2X/7/70f90addd7883759e4a0d06a934946c2be8aa6c1.fzpz).

## How It Works

1. **Initialization**: The Arduino initializes the two LCD screens.
2. **Joystick Navigation**: The joystick is used to navigate the keyboard displayed on the first LCD screen. The joystick's x and y axes move the cursor, and the switch selects the current character.
3. **Debouncing**: Software debouncing is implemented to handle the mechanical bouncing of the joystick switch.
4. **Keyboard Input**: The keyboard can switch between lowercase and uppercase letters, changing some characters (e.g., '.' to '?', ',' to '!').
5. **Display Text**: The typed text is displayed on the second LCD screen.

## Usage

1. **Connect Components**: Assemble the components as per the Fritzing diagram.
2. **Upload Code**: Upload the provided `.ino` file to your Arduino Uno.
3. **Monitor Serial**: Open the Serial Monitor in your Arduino IDE to see potential debug messages.
4. **Navigate Keyboard**: Use the joystick to navigate the keyboard and press the joystick switch to select characters.
5. **Switch Case**: Use the designated key on the keyboard to switch between lowercase and uppercase letters.

## Acknowledgments

- **LiquidCrystal_I2C Library**: Used for interfacing with the I2C LCD screens.
- **Fritzing Parts**: LCD screen I2C component from the provided link.