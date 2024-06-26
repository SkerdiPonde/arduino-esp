# Arduino Distance Sensor with Buzzer and LCD Display

### Author: Skerdi Ponde

This project utilizes an Arduino Uno (ATmega328P) to create a distance sensor using an HC-SR04 ultrasonic sensor. It includes an active buzzer (KY-012) for auditory feedback and a 16x2 I2C LCD screen to display the distance measurements.

## Components Used

- **Arduino Uno (ATmega328P)**
- **HC-SR04 Ultrasonic Sensor**
- **KY-012 Active Buzzer**
- **16x2 I2C LCD Screen** (LiquidCrystal_I2C library)
- **Breadboard and Jumper Wires**

## Fritzing Diagram

In the Fritzing diagram, the KY-012 active buzzer component was downloaded from [Arduino Modules](https://arduinomodules.info/download/ky-012-active-buzzer-module-zip-file/) by [Arduino Modules](https://arduinomodules.info/).

For the I2C module, the **LCM1602 IIC** model was used, and the part was downloaded from this [link](https://fritzing.org/projects/lcd-i2c-analog-inputoutput-displayed).

## How It Works

1. **Initialization**: The Arduino Uno initializes connections to the HC-SR04 sensor, KY-012 buzzer, and 16x2 I2C LCD screen.
2. **Distance Measurement**: The HC-SR04 sensor measures distances based on sound wave reflection.
3. **Median Filtering**: A moving median filter (based on 5 readings) is applied to smooth noisy distance measurements.
4. **Buzzer Feedback**: The KY-012 buzzer emits sounds proportional to the distance of detected objects.
5. **LCD Display**: The 16x2 I2C LCD screen displays the current distance to an obstacle in centimeters.


## Usage

1. **Setup**:
   - Connect the components as per the Fritzing diagram.
   - Ensure the Arduino Uno is connected to power via USB.
   
2. **Upload Code**:
   - Upload the provided `.ino` file to your Arduino Uno using the Arduino IDE.

3. **Monitor Serial**:
   - Open the Serial Monitor in the Arduino IDE to view distance measurements.

4. **View LCD Display**:
   - The 16x2 I2C LCD screen will display the current distance in centimeters.

## Acknowledgments

- **LiquidCrystal_I2C Library**: Used for interfacing with the I2C LCD screen.
- **I2C Module (LCM1602 IIC)**: Part downloaded from this [link](https://fritzing.org/projects/lcd-i2c-analog-inputoutput-displayed).
- **HC-SR04 Ultrasonic Sensor**: Provides distance measurement capabilities.
- **KY-012 Active Buzzer**: Provides auditory feedback based on distance.  Part downloaded from [Arduino Modules](https://arduinomodules.info/download/ky-012-active-buzzer-module-zip-file/)
