#include <LiquidCrystal_I2C.h>

// Buzzer pin
const int buzzPin = 13;

// HC-SR04 ultrasonic sensor pins
const int trigPin = 7;
const int echoPin = 8;

// // LCD screen pins
// const int sda = A4;
// const int scl = A5;

// will keep track of time for the buzzers
unsigned long currentMillis; 
unsigned long previousMillis = 0;

// on time and off time for the buzzer, off will vary
const long onInterval = 100;
long offInterval;

// HC-SR04 reading interval
const long sensorInterval = 50;
unsigned long previousReading = 0;

// LCD interval
const long lcdInterval = 500;
unsigned long previousPrint = 0;

// 5 values for the moving median to filter noisy readings
const int numReadings = 3;
int readings[numReadings];
int sorted[numReadings];
int index = 0;

// Distance and median distance of numReading measurements
int distance;
int medianDistance = 0;
int prevMedianDistance = 0;

// Initial buzzer state
bool buzzState = LOW;

// LCD screen
LiquidCrystal_I2C lcd(0x27, 16, 2);

// LCD print flag
bool printFlag = false;

void setup() {
  // initalize serial communication to view the data
  Serial.begin(9600);

  // initialize trigPin as an output pin as it will send the pulse
  // to trigger the sensor and echoPin as input as it will read the
  // voltage produced by the sensor when the sound wave returns
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // // initialize lcd pins and screen
  // pinMode(sda, OUTPUT);
  // pinMode(scl, OUTPUT);

  lcd.init();
  lcd.backlight();

  // Write distance on the first row
  lcd.setCursor(0, 0);
  lcd.print("Distance:");

  // initialize buzzerPin as output
  pinMode(buzzPin, OUTPUT);
  
  // Popluate the arrays with 0s initially
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 1000;
    sorted[i] = 1000;

  }
}

void loop() {
  // Current time in milliseconds
  currentMillis = millis();

  // Check if its time to read from the sensor
  if (currentMillis - previousReading >= sensorInterval) {
    // Read the distance from the sensor
    distance = readDistance();
    
    // Store the new reading in the array
    readings[index] = distance;

    // Sort the values in the readings array
    memcpy(sorted, readings, numReadings * sizeof(int));
    qsort(sorted, numReadings, sizeof(int), compare);
    
    // Calculate the median
    medianDistance = sorted[numReadings / 2];

    // Check if it is different from the previous value
    printFlag = (medianDistance != prevMedianDistance);

    // Print medianDistance on the serial monitor
    Serial.print(medianDistance);
    Serial.println(" cm");

    // Update the previous reading time
    previousReading = currentMillis;

    // Update the offInterval time
    calculateInterval(medianDistance);

    // Store the previous distance and only refresh the shown val if the current
    // value is different from the previous
    prevMedianDistance = medianDistance;
  }
  // Serial.println(medianDistance);

  // Turn buzzer on or off
  useBuzzer();

  // Print distance on lcdScreen
  if (currentMillis - previousPrint >= lcdInterval && printFlag) {
    printDist(medianDistance);
    previousPrint = currentMillis;
  } 

  // Move to the next index
  index = (index + 1) % numReadings;
  
}

// Function to read the distance from the sensor
int readDistance() {
  // duration will store the time it takes for the sound to come back
  // inches and cm will be the distances to the obstacle in inches and cm
  long duration, inches, cm;

  // Make sure the voltage at trigPin is LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a pulse that lasts 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // expected pulse duration for HC-SR04
  digitalWrite(trigPin, LOW);

  // echoPin will turn HIGH when the sound wave is sent and it will stay that way
  // until the sound wave returns after hitting an object
  // the duration in microseconds of the HIGH signal from echoPin is measured with pulseIn
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = msToIn(duration);
  cm = msToCm(duration);

  // // print the values in the serial monitor
  // Serial.print(inches);
  // Serial.print("in, ");
  // Serial.print(cm);
  // Serial.print("cm");
  // Serial.println();

  return cm;
}


// Compare function needed by qsort
int compare(const void *a, const void *b) {
  return (*(int*)a - *(int*)b);
}


// Convert sound travel time in ms to inches
// Speed of sound in air is roughly 340 m/s
long msToIn(long microseconds) {
  // 0.0134 in is the distance sound travels in a microsecond
  // also divide by 2 since the sound travels double the distance
  return microseconds * 0.0134 / 2;
}

// Convert sound travel time in ms to cm
long msToCm(long microseconds) {
  // 0.034 cm is the distance sound travels in a microsecond
  // also divide by 2 since the sound travels double the distance
  return microseconds * 0.034 / 2;
}

// Calculate the off interval based on the distance of the obstacle to the sensor
// The closer the object the more frequet the beeps (smaller offInterval)
void calculateInterval(int dist) {
  // Map the distance to the obstacle to a time value for the off interval of the buzzer
  long unconstrained = map(dist, 4, 40, 20, 800);
  offInterval = constrain(unconstrained, 20, 800);
  // Serial.print(dist);
  // Serial.print(" cm, ");
  // Serial.println(offInterval);
}

// Function to turn buzzer on or off
void useBuzzer() {
  // If the distance to the obstacle is larger than 40 cm, then turn the buzzer off
  if (medianDistance >= 40) {
    // Serial.println("HERE");
    buzzState = LOW;
    digitalWrite(buzzPin, LOW);
  }
  // Otherwise
  else {
    // If the buzzer is on and the defined on time has been reached/exceeded turn
    // it off, and update previousTime (since buzzer is flipped)
    if (buzzState == HIGH && currentMillis - previousMillis >= onInterval) {
      // Serial.println(currentMillis - previousMillis);
      buzzState = LOW;
      digitalWrite(buzzPin, LOW);
      previousMillis = currentMillis;
    }
    // If it is off and the off time has been reached/exceeded turn it on, and 
    // update previousTime
    else if (buzzState == LOW && currentMillis - previousMillis >= offInterval) {
      // Serial.println(currentMillis - previousMillis);
      buzzState = HIGH;
      digitalWrite(buzzPin, HIGH);
      previousMillis = currentMillis;
    }

  }
}

// Function to clear the second row of the LCD screen
// (where the distance in cm is printed)
void clearRow(int row) {
  lcd.setCursor(0, row); // Set cursor to 0th col of row
  for (int i = 0; i < 8; i++) {
    lcd.print(" "); // Print whitespaces
  }
  lcd.setCursor(0, row); // Set cursor to the 0th col of the row again
}

// Print the distance in cm on the LCD screen
void printDist(int dist) {
  clearRow(1);
  lcd.print(dist);
  lcd.print(" cm");
}