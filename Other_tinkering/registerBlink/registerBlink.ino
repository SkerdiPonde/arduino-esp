#include <avr/io.h>
#include <util/delay.h>

// LED blink on pin 13 using registers
int main() {
  Serial.begin(9600);
  // Set pin 13 (PB5) to output
  DDRB = 0b00100000;
  // Initially set its voltage to LOW
  PORTB = 0b00000000;

  while(1) {
    // Flip the bit
    PORTB ^= 1 << 5;
    _delay_ms(500);
  }
}
