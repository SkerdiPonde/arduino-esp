#include <avr/io.h>

// Check the ATmega328p datasheet om how to set up PWM
void setup_pwm() {
  // Set pin 9 (OC1A) as output and connect an LED to it
  DDRB |= (1 << DDB1);
  PORTB = 0b00000000;

  // Configure a Timer Interrupt
  TCCR1A = 0b00000000; // Clear the TCCR1A register
  TCCR1B = 0b00000000; // Clear TCCR1B as well

  TCNT1 = 0; // Clear the counter

  // Configure Timer 1 for Fast PWM, non-inverted mode
  TCCR1A = (1 << COM1A1) | (1 << WGM11) | (1 << WGM10); // Fast PWM 10-bit, non-inverting mode
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // Fast PWM, prescaler 64

  // Initial duty cycle: 50% (512)
  OCR1A = 512;

  // This corresponds to a period of 0.004096 seconds
  // = (1/16MHz) per clock step * 1024 (10-bit) steps for the whole cycle to repeat * 64 prescaler
  // This corresponds to a frequency of 244.14 Hz

  // Note: I attempted setting OCR1A value to 0, expecting the LED to be off (0% duty cycle), however
  // it still had a faint glow. I am assuming the issue is, that no comparison occurs when the clock resets
  // to step 0 (TCNT1 = 0), and it it occurs when TCNT1 = 1 instead. Since TCNT1 has exceeded the value of
  // OCR1A that is when the clear of OC1A occurs.

  // Using inverted mode instead and setting OCR1A to 1023 does indeed lead to the LED being off the entire time
  // as expected. I am assuing that setting it to 0 would produce the same result as with OCR1A = 1, similar to 
  // non-inverted mode, however there is no way of telling since both are very bright and indistinguishable.

  // Phase corrected PWM might mitigate this issue, but I haven't tested.
}

int main() {
  setup_pwm();
  // Main loop
  while (1) {
    // Do nothing
  }
  return 0;
}
