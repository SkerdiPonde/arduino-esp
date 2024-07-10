// Joystick pins
const int joySwitch = 2;
const int xPin = A0;
const int yPin = A1;

void setup() {
  // Set joystick switch pin to input with a pull up resistor
  pinMode(joySwitch, INPUT_PULLUP);

  // Begin serial communication
  Serial.begin(9600);
}

void loop() {
  // Print info
  Serial.print("Switch: ");
  Serial.println(digitalRead(joySwitch));
  Serial.print("X: ");
  Serial.println(analogRead(xPin));
  Serial.print("Y: ");
  Serial.println(analogRead(yPin));
  Serial.println();
  delay(100);
}
