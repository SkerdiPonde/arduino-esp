// Pin to drive the motor forward
int forwardPin = 5;
// Pin connected to potentiometer to change the speed
int potPin = A0;

void setup() {
  Serial.begin(9600);
  // Set forwardPin to output
  pinMode(forwardPin, OUTPUT);

}

void loop() {
  // Read potentiometer values
  int potVal = analogRead(potPin);
  // Map to analog output values
  int motorVal = map(potVal, 0, 1023, 150, 255);
  // Print on serial monitor
  Serial.println(motorVal);
  // Set value to forwardPin
  analogWrite(forwardPin, motorVal);
  delay(50);
}
