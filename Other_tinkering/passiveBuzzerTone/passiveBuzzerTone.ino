int buzzerPin = 13;
int buttonPin = 2;
int buttonState;
int delayTime = 500;

void setup() {
  // Set the buzzer pin to output mode and button pin to input
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  // Play tones when button is pressed 
  // tone(pin, freq) sets a 50% duty cycle voltage (V) on pin with a frequency freq (Hz)
  if(buttonState == HIGH) {
    tone(buzzerPin, 400);
    delay(delayTime);

    tone(buzzerPin, 500);
    delay(delayTime);

    tone(buzzerPin, 600);
    delay(delayTime);

    tone(buzzerPin, 700);
    delay(delayTime);

    tone(buzzerPin, 800);
    delay(delayTime);

    tone(buzzerPin, 900);
    delay(delayTime);

    tone(buzzerPin, 1000);
    delay(delayTime);

    tone(buzzerPin, 1200);
    delay(delayTime);

    tone(buzzerPin, 1400);
    delay(delayTime);

    tone(buzzerPin, 1600);
    delay(delayTime);

    tone(buzzerPin, 1800);
    delay(delayTime);

    tone(buzzerPin, 2000);
    delay(delayTime);

    tone(buzzerPin, 2500);
    delay(delayTime);

    tone(buzzerPin, 3000);
    delay(delayTime);

    tone(buzzerPin, 3500);
    delay(delayTime);

    tone(buzzerPin, 4000);
    delay(delayTime);

    tone(buzzerPin, 4500);
    delay(delayTime);
  }
  // If button is not pressed play no tone
  else {
    noTone(buzzerPin);
  }
}
