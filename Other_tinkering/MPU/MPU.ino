#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Create an Adafruit_MPU6050 instance
Adafruit_MPU6050 mpu;

// These variables will store the offsets of the gyroscope
float gyroX_offset = 0;
float gyroY_offset = 0;
float gyroZ_offset = 0;

// Store the angular velocities around each axis
float rot_x, rot_y, rot_z;

// Store the value of the angle around z axis
float angle = 0;

// Keep track of the time when each reading occurs
unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 10;


void setup() {
  // Wait for serial communication to start
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  Serial.println("MPU test!");

  // Initialize the MPU6050
  initializeMPU6050();

  Serial.println("Calibrating gyro ...");

  // Calibrate the gyro using the average offsets of 2000 readings
  calibrateGyro(2000);

  Serial.println("Calibration finished");

}

void loop() {
  currentMillis = millis();
  // Every 10 ms 
  if (currentMillis - previousMillis >= interval) {
    if (!mpu.begin()) {  // Check if MPU6050 is responding
      Serial.println("MPU6050 not found, reinitializing...");
      initializeMPU6050();  // Reinitialize MPU6050
    }
    else {
      // Get new sensor events with the readings 
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
      
      // Calibrate sensor values
      // rot_x = g.gyro.x - gyroX_offset;
      // rot_y = g.gyro.y - gyroY_offset;

      // This was an attempt to measure the angle of orientation of the sensor around
      // the z-axis, however this is prone to drift and quickly becomes highly inaccurate,
      // hence a filter should be used
      rot_z = g.gyro.z - gyroZ_offset;
      angle += rot_z*57.3*(currentMillis+3-previousMillis)*0.001; // Convert to deg
      Serial.println(angle);
      
      
    }
    // Update the time the previous reading was made
    previousMillis = currentMillis;
  }

}

// Function to initialize the MPU6050
// Noticed that any loose connections would disrupt the readings and
// the sensor has to be reinitialized to be able to produce plausible values again
void initializeMPU6050() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }
  else {
    Serial.println("MPU6050 Found!");
    // Use these methods from the Adafruit library to initialize
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
  }

  Serial.println("");
  delay(100);
}

// Function to calibrate the gyroscope
// Make sure it is stationary
void calibrateGyro(int numReadings) {
  // Variables to store the sum of all readings
  float gx, gy, gz;

  // Take numReadings readings in total
  for (int i = 0; i < numReadings; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    gx += g.gyro.x;
    gy += g.gyro.y;
    gz += g.gyro.z;
    delay(2);
  }

  // Calculate the offset as the mean
  gyroX_offset = gx / numReadings;
  gyroY_offset = gy / numReadings;
  gyroZ_offset = gz / numReadings;
}