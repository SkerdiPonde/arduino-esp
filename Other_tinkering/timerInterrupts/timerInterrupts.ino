#include <avr/io.h>
#include <avr/interrupt.h>

int main() {
 
  // PB1 is configured as output and is initially set to LOW
  // Same for PB0
  DDRB = 0b00000011;
  PORTB = 0b00000000;

  cli(); // Disable global interrupts

  // Configure a Timer Interrupt
  TCCR1A = 0b00000000; // Clear the TCCR1A register
  TCCR1B = 0b00000000; // Clear TCCR1B as well

  TCNT1 = 0; // Clear the counter
  OCR1A = 15625; // Value reached after 1 sec

  TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (PB0 - Pin9)
  TCCR1B |= (1 << WGM12); // CTC on MAX value of OCR1A 
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024

  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  
  sei(); // Enable global interrupts

  while(1) {
    // Empty loop
  }
}

ISR(TIMER1_COMPA_vect) {
  // Code to execute during interrupt routine
  // PORTB ^= (1 << PB1); This is not needed as OC1A is toggled by default
  // Maybe toggle pin 8
  PORTB ^= (1 << PB0);
}

