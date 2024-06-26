#include <LiquidCrystal_I2C.h>
#include <math.h>

int potVal; // Poetentiometer value analog 0-1023
int lcdVal; // Mapped value of the potentiometer to match the LCD screen dims
int prevLcdVal; // Previous value to compare
int cursorPos; // Current position index to print a block

// Create a LiquidCrystal_I2C instance
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Each LCD screen cell has 8 rows and 5 columns, so an array of type
// block is declared with 8 "bytes" of 5 bits each, all set to 1 to
// represent a solid block
byte solidBlock[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

// Setup
void setup() {
  // Begin serial communication
  Serial.begin(9600);

  // Set the mode of pin A0 to input as it will be reading potentiometer vals
  pinMode(A0, INPUT);

  // Initialize the LCD screen
  lcd.init();
  lcd.backlight();

  // Starting on first col (0) of first row (0) print "Volume:"
  lcd.setCursor(0, 0);
  lcd.print("Volume:");

  // Create a the custom char and store it in pos 0 of CGRAM
  lcd.createChar(0, solidBlock);
  
  // This will store the position of the cursor in row 1
  cursorPos = 0;

  // Read the initial value of the potentiometer
  potVal = analogRead(A0);

  // Map it to value between 0-16, which is the number of blocks that 
  // will be displayed on the screen
  lcdVal = round(16.0 * (potVal / 1023.0));
  //Serial.println(lcdVal);

  // For loop to write the blocks on the screen
  for (int i = 0; i < lcdVal; i++) {
    lcd.setCursor(cursorPos, 1);
    lcd.write(byte(0)); // Write the char stored in position 0 of CGRAM
    cursorPos++; // Increment cursor position
    
  }
  // Store the value as the previous LCD value for reference
  prevLcdVal = lcdVal;
}

// Infinite loop
void loop() {
  // Read the potentiometer value and map it to a lcdVal
  potVal = analogRead(A0);
  lcdVal = round(16.0 * (potVal / 1023.0));
  // Serial.println(16.0 * (potVal / 1023.0));
  // Serial.print(lcdVal);
  // Serial.print(", ");
  // Serial.print(prevLcdVal);
  // Serial.print(", ");
  // Serial.println(cursorPos);

  // If lcdVal is larger than prevLcdVal, then write as many blocks on the screen
  // as the difference between the two
  if (lcdVal > prevLcdVal) {
    for (int i = 0; i < lcdVal - prevLcdVal; i++) {
      lcd.setCursor(cursorPos, 1); // Set cursor to the correct position
      lcd.write(byte(0)); // Write the char stored in position 0 of CGRAM
      cursorPos++; // Increment cursor position
    }
  }

  // Otherwise "delete" the blocks
  else if (lcdVal < prevLcdVal) {
    for (int i = 0; i < prevLcdVal - lcdVal; i++) {
      cursorPos--; // Decrement cursorPos position
      lcd.setCursor(cursorPos, 1); // Set the cursor to this previous position
      lcd.write(' '); // Rewrite it to a white space
    }
  }
  
  // Store the previous val
  prevLcdVal = lcdVal;
  delay(50);
}
