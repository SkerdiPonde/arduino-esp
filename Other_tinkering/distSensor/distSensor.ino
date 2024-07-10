const int trigPin = 7;
const int echoPin = 8;

void setup() {
  // initalize serial communication to view the data
  Serial.begin(9600);

  // initialize trigPin as an output pin as it will send the pulse
  // to trigger the sensor and echoPin as input as it will read the
  // voltage produced by the sensor when the sound wave returns
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
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

  // print the values in the serial monitor
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100); 
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
