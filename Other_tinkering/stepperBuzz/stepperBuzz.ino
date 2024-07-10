// Code was written following the guide from https://youtu.be/avrdDZD7qEQ
// by https://www.youtube.com/@nikodembartnik

#include <avr/interrupt.h>

// In1, In2, In3, In4 of stepper motor driver
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

// Pin where active buzzer is connected
#define BUZZER_PIN 2

// Step number within a cycle 0, 1, 2, 3 (4 steps per cycle)
int step_number = 0;

// Variable to count the steps
int count = 0;

// Store the state of the buzzer
volatile bool beep = false;

void setup() {
  Serial.begin(9600);

  // Set motor and buzzer pins to output 
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  // Decided to use a timer interrupt to control the beep duration independently
 
  // Setup Timer1 for CTC interrupt
  cli(); // Disable global interrupts
  TCCR1A = 0; // Clear Timer1 Control Register A
  TCCR1B = 0; // Clear Timer1 Control Register B
  TCNT1 = 0; // Initialize Timer1 counter
  TCCR1B |= (1 << WGM12); // Enable CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  OCR1A = 2000; // Set compare match register for 128 ms
  sei(); // Enable global interrupts
}

void loop() {
  // Take a step counterclockwise
  OneStep(true);
  // Increment the count of steps
  count++;
  // This motor has 2048 steps/rev when the microstepping capability
  // is not being used
  if (count == 2048) {
    count = 0; // Reset count
    beep = true; // Set true since the buzzer will be making noise
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("BEEP");
    TCNT1 = 0; // Reset Timer1 counter
  }
  delay(2);
}


// Function to rotate the motor shaft one step
void OneStep(bool direction) {
  // Counterclockwise if true
  if (direction) {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;    
    }
  }
  // Clockwise if false
  else {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break; 

      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;   
    }

  }
  // Increment step number
  step_number++;
  // Wrap around
  if (step_number > 3) {
    step_number = 0;
  }
}

// When TCNT1 reaches OCR1A Timer1 compare match interrupt is triggered
ISR(TIMER1_COMPA_vect) {
  // If the buzzer is beeping
  if (beep) {
    digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
    beep = false; // Reset beep flag
  }
}

