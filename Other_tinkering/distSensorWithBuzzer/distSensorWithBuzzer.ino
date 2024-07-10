const int trigPin = 7;
const int echoPin = 8;
const int buzzerPin = 13;

// 5 values for the moving median to filter noisy readings
const int numReadings = 5;
int readings[numReadings];
int sorted[numReadings];
int index = 0;

void setup() {
  // initalize serial communication to view the data
  Serial.begin(9600);

  // initialize trigPin as an output pin as it will send the pulse
  // to trigger the sensor and echoPin as input as it will read the
  // voltage produced by the sensor when the sound wave returns
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // initialize buzzerPin as output
  pinMode(buzzerPin, OUTPUT);

  // Popluate the arrays with 0s initially
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
    sorted[i] = 0;
  }

}

void loop() {
  // Read the distance from the sensor
  int distance = readDistance();
  
  // Store the new reading in the array
  readings[index] = distance;

  // Sort the values in the readings array
  memcpy(sorted, readings, numReadings * sizeof(int));
  qsort(sorted, numReadings, sizeof(int), compare);
  
  // Calculate the median
  int medianDistance = sorted[numReadings / 2];

  // Update the buzzer frequency;
  updateBuzzerFrequency(medianDistance);

  // Print the median distance on screen
  Serial.print(medianDistance);
  Serial.println("cm");

  // Move to the next index
  index = (index + 1) % numReadings;

  delay(100); 
}

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

void updateBuzzerFrequency(int distance) {
  // Set a tone based on the distance of the obstacle to the sensor
  if (distance == 0) {
    noTone(buzzerPin);
  }
  else if (distance <= 5) {
    tone(buzzerPin, 3000);
  }
  else if (distance <= 10) {
    tone(buzzerPin, 1600);
  }
  else if (distance <= 20) {
    tone(buzzerPin, 1200);
  }
  else if (distance <= 30) {
    tone(buzzerPin, 600);
  }
  else if (distance <= 40) {
    tone(buzzerPin, 200);
  }
  else {
    noTone(buzzerPin);
  }
  // int freq = map(distance, 5, 50, 1500, 0);
  
  // if (freq <= 0 || freq > 1500) {
  //   noTone(buzzerPin);
  // }
  // else {
  //   tone(buzzerPin, freq);
  // }
  
}

// Compare function needed by qsort
int compare(const void *a, const void *b) {
  return (*(int*)a - *(int*)b);
}


// Speed of sound in air is roughly 340 m/s
long msToIn(long microseconds) {
  // 0.0134 in is the distance sound travels in a microsecond
  // also divide by 2 since the sound travels double the distance
  return microseconds * 0.0134 / 2;
}

long msToCm(long microseconds) {
  // 0.034 cm is the distance sound travels in a microsecond
  // also divide by 2 since the sound travels double the distance
  return microseconds * 0.034 / 2;
}
