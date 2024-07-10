// Pins to control the speed and direction of motor 1
int forwardPin1 = 5; 
int backwardPin1 = 6;

// Pins to control the speed and direction of motor 2
int forwardPin2 = 7;
int backwardPin2 = 8;

void setup() {
  // Set pins as output
  pinMode(forwardPin1, OUTPUT);
  pinMode(backwardPin1, OUTPUT);

  pinMode(forwardPin2, OUTPUT);
  pinMode(backwardPin2, OUTPUT);

  // Write High to one of them to see if the motors have been
  // connected correctly
  digitalWrite(forwardPin1, HIGH);
  digitalWrite(forwardPin2, HIGH);
}

void loop() {
}
