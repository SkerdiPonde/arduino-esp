int pmPin = A0;
int buzzerPin = 13;
int pmVal;

void setup() {
  Serial.begin(9600);
  pinMode(pmPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
  // Read the value from the potentiometer
  pmVal = analogRead(pmPin);
  Serial.println(pmVal);
  int freq = map(pmVal, 0, 1023, 2000, 400);
  // Play a tone based on the analog value produced by the potentiometer
  tone(buzzerPin, freq);
}
