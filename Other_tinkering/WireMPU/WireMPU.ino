// This piece of code was written following the guide from 
// https://youtu.be/yhz3bRQLvBY and https://youtu.be/Yh6mYF3VdFQ by
// https://www.youtube.com/@carbonaeronautics

#include <Wire.h> // Library for I2C communication

float RateRoll, RatePitch, RateYaw; // Declare global variables

// Calibration variables
float RateCalibrationRoll = 0, RateCalibrationPitch = 0, 
      RateCalibrationYaw = 0; 

// 2000 readings to perform the calibration
int RateCalibrationNumber = 2000;

// Function to read measurements from the gyroscope of the sensor
void gyro_signals(void) {
  // Start I2C communication (Default address 0x68 in documentation)
  Wire.beginTransmission(0x68); 

  Wire.write(0x1A); // Digital low pass filter register address
  Wire.write(0x05); // DLPF_CFG value  to set bandwidth of 10
  Wire.endTransmission();

  Wire.beginTransmission(0x68); 
  Wire.write(0x1B); // Register to configure gyroscope scale factor
  Wire.write(0x8);  // Value to set full scale 500 deg/s and LSB sensitivity to 65.5 LSB/deg/s
  Wire.endTransmission();

  Wire.beginTransmission(0x68); 
  Wire.write(0x43); // Address of first register that holds gyro measurements
  Wire.endTransmission();

  Wire.requestFrom(0x68, 6); // Request the 6 bytes following the address to get gyro vals
  
  // Read gyro measurements
  int16_t GyroX = Wire.read() << 8 | Wire.read();
  int16_t GyroY = Wire.read() << 8 | Wire.read(); 
  int16_t GyroZ = Wire.read() << 8 | Wire.read(); 

  // Divide by the LSB sensitivity value
  RateRoll = (float)GyroX/65.5;
  RatePitch = (float)GyroY/65.5;
  RateYaw = (float)GyroZ/65.5;
}

// Function to calibrate the gyroscope
void gyro_calibration() {
  for (int i = 0; i < RateCalibrationNumber; i++) {
    gyro_signals();
    RateCalibrationRoll += RateRoll;
    RateCalibrationPitch += RatePitch;
    RateCalibrationYaw += RateYaw;
    delay(1);
  }
  // Calculate the average offset
  RateCalibrationRoll /= RateCalibrationNumber;
  RateCalibrationPitch /= RateCalibrationNumber;
  RateCalibrationYaw /= RateCalibrationNumber;
}

bool checkConnection() {
  Wire.beginTransmission(0x68);
  Wire.write(0x75); // WHO_AM_I register address

  if (Wire.endTransmission() != 0) {
    return false; // I2C transmission error
  }

  Wire.requestFrom(0x68, 1); // Request 1 byte
  if (Wire.available() == 1) { 
    uint8_t whoAmI = Wire.read();
    if (whoAmI == 0x68) {
      return true; // Sensor is connected
    }
  }
  return false; // Sensor is not responding 
}

void initializeSensor() {
  Wire.setClock(400000); // 400 kHz from product specifications
  Wire.begin();
  delay(250);

  Wire.beginTransmission(0x68);
  Wire.write(0x6B); // Start the gyro in power mode
  Wire.write(0x00); // Set all values to 0
  Wire.endTransmission();
}

void setup() {
  // Begin serial communication
  Serial.begin(115200);

  Wire.setClock(400000); // 400 kHz from product specifications
  Wire.begin();
  delay(250);

  Wire.beginTransmission(0x68);
  Wire.write(0x6B); // Start the gyro in power mode
  Wire.write(0x00); // Set all values to 0
  Wire.endTransmission();

  // Calibrate gyroscope
  gyro_calibration();
}

void loop() {
  // If something went wrong reinitialize the sensor
  if (!checkConnection()) {
    Serial.println("Lost connection!");
    initializeSensor();
    
  }
  else {
    // Read angular velocities from the gyro
    gyro_signals();
    
    // Adjust using the offsets
    RateRoll -= RateCalibrationRoll;
    RatePitch -= RateCalibrationPitch;
    RateYaw -= RateCalibrationYaw;

    // Print on serial monitor
    Serial.print("Roll rate [deg/s]= ");
    Serial.print(RateRoll);
    Serial.print(" Pitch rate [deg/s]= ");
    Serial.print(RatePitch);
    Serial.print(" Yaw rate [deg/s]= ");
    Serial.println(RateYaw);

    delay(50);
  }
}
