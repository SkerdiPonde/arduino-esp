// Check the datasheet of 5461AS for more information on the pin layout
// Pin 10 - Digit 1
// Pin 11 - Digit 2
// Pin 12 - Digit 3
// Pin 13 - Digit 4

// Pin 2 - Segment A
// Pin 3 - Segment B
// Pin 4 - Segment C
// Pin 5 - Segment D
// Pin 6 - Segment E
// Pin 7 - Segment F
// Pin 8 - Segment G
// Pin 9 - Decimal Point

// SevSeg library is used for this
#include "SevSeg.h"
SevSeg display;

// Keep track of the time the previous number was displayed
// to display the new number after a certain interval
unsigned long currMillis;
unsigned long prevMillis = 0;
const unsigned long interval = 100;

// Timer
int count = 0;

void setup() {
  // Number of digits the display has
  byte digits = 4;
  // Pins the digits are connected to D1, D2, D3, D4
  byte digitPins[digits] = {10, 11, 12, 13};

  // Pins the segments are connected to A, B, C, D, E, F, G, DP
  byte segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

  // 1kOhm resistors are placed between the the Arduino pins and digit
  // pins of the display
  byte digitResistors = true;

  // Define configuration to be common cathode or anode based on the type of display
  byte config = COMMON_CATHODE;

  display.begin(config, digits, digitPins, segmentPins, digitResistors);
}

void loop() {
  // Polling
  currMillis = millis();
  if (currMillis - prevMillis >= interval) {
    prevMillis = currMillis;
    // Display count
    // Decimal point to the left of the rightmost digit
    display.setNumber(count, 1);
    // Increment count
    count++;
  }
  // Refresh display
  display.refreshDisplay();
}
