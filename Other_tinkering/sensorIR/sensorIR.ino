int pinIR = 3;

void setup() {
  Serial.begin(9600);
  // Set infrared sensor pin to input
  pinMode(pinIR, INPUT);
}

void loop() {
  // Read the digital value from the sensor pin
  int val = digitalRead(pinIR);
  // The particular sensor used would produce a LOW voltage
  // when there is an object in front of it. When this object 
  // had a black color however, the sensor would not trigger
  // or would trigger only in very close proximity. This is due
  // to the properties of the color black. Objects with this color
  // absorb more IR light, hence the portion that is reflected is
  // not enough to trigger the sensor. 

  // The sensor has a potentiometer that can be used to adjust the
  // sensitivity
  Serial.println(val);
  delay(100);
}
