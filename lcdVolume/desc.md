# Arduino Volume Bar Display

## Author: Skerdi Ponde

This project uses an Arduino Uno (Atmega328p) to display a "volume" bar on a 16x2 I2C LCD screen. The volume bar is controlled by a WH148 rotary potentiometer, but note that it doesn't actually control the volume of any device; it is simply a visual representation.

## Components Used

- **Arduino Uno (Atmega328p)**
- **16x2 I2C LCD Screen** (LiquidCrystal_I2C library)
- **WH148 Rotary Potentiometer**
- **Breadboard and Jumper Wires**

## Fritzing Diagram

For the I2C module, the **LCM1602 IIC** model was used, and the part was downloaded from this [link](https://fritzing.org/projects/lcd-i2c-analog-inputoutput-displayed).

## How It Works

1. **Initialization**: The Arduino initializes the LCD screen and prints "Volume:" on the first row.
2. **User Input**: The potentiometer's position is read and mapped to the LCD screen width.
3. **Display Volume Bar**: The mapped value is displayed as a series of solid blocks on the second row of the LCD screen, visually representing the volume.

## Usage

1. **Connect Components**: Assemble the components as per the Fritzing diagram.
2. **Upload Code**: Upload the provided `.ino` file to your Arduino Uno.
3. **Monitor Serial**: Open the Serial Monitor in your Arduino IDE to see the potentiometer values.
4. **Adjust Potentiometer**: Rotate the WH148 rotary potentiometer to change the displayed volume bar on the LCD screen.

## Acknowledgments

- **LiquidCrystal_I2C Library**: Used for interfacing with the I2C LCD screen.
- **Solid Block Character**: Custom character created for displaying the volume bar.
- **LCM1602 IIC**: Part downloaded from this [link](https://fritzing.org/projects/lcd-i2c-analog-inputoutput-displayed).