#include <LiquidCrystal_I2C.h>

// Screen to display the typed text
LiquidCrystal_I2C lcdtext(0x27, 16, 2); 
// Screen to display characters and symbols -> keyboard
LiquidCrystal_I2C lcdkey(0x23, 16, 2);

// Create a character arrays to hold the lowercase and uppercase
// letters respectively
// 0th "page" will store the lowercase letters
// 1st "page" will store the uppercase letters
char keyboard[2][2][16];

// Store the current keyboard configuration uppercase or lowercase
int keyboardType = 0;

// Cursor position on keyboard lcd screen
int row = 0;
int col = 0;

// Cursor postion on text lcd screen
int rowText = 0;
int colText = 0;

// Cursor position change interval and last time it changed in ms
const unsigned long cursorInterval = 150;
unsigned long lastChange = 0;
// Store current time in ms
unsigned long currentMillis;

// Store the text that's written
String text = "";

// Custom character to denote the change of letters to uppercase
byte upArrow[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};
// Custom character to denote the change of letters to lowercase
byte downArrow[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000
};
// Custom character for the backspace
byte backspace[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01000,
  0b11111,
  0b01000,
  0b00100,
  0b00000
};
// Custor character for enter
byte enter[8] = {
  0b00100,
  0b01000,
  0b11111,
  0b01001,
  0b00101,
  0b00001,
  0b00001,
  0b00000
};


// Joystick pins
const int buttonPin = 2;
const int xPin = A0;
const int yPin = A1;

// Store the joystick button state
bool buttonState = HIGH;
bool lastButtonState = HIGH;

// Joystick button debounce timer
const unsigned long debounceDelay = 40;
unsigned long lastDebounceTime = 0;

bool buttonPressed = false;


void setup() {
  // Begin serial communication
  Serial.begin(9600);

  // Initialize the lcd screens
  lcdtext.init();
  lcdtext.backlight();
  lcdkey.init();
  lcdkey.backlight();
  

  lcdkey.createChar(0, upArrow);
  lcdkey.createChar(1, downArrow);
  lcdkey.createChar(2, backspace);
  lcdkey.createChar(3, enter);

  // Fill the arrays with values
  fillKeyboardLower();
  fillKeyboardUpper();
 
  // Initialize joystick's switch pin as an input with a pull up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  lcdkey.setCursor(col, row);
  // Make the cursor blink on the keyboard lcd
  lcdkey.blink();
  lcdtext.setCursor(colText, rowText);
  // Make the cursor blink on the keyboard lcd
  lcdtext.blink();

  updateKeyScreen(keyboardType);

}

void loop() {
  // Call function to place the cursor on the correct position based
  // on the joystick input
  placeCursor();

  // Call function to check if button has been pressed
  checkButton();
  
  // A button press is determined by the previous function
  if (buttonPressed) {
    // Call the function that manages the text string saved internally
    joyButtonEvent(keyboardType);
    // Update the text  on the lcd screen in case a character is added or removed
    updateTextScreen(keyboardType);
  }

  // Reset the state
  buttonPressed = false;
}


// Functions to fill the character arrays with values
// Fill for lowercase
void fillKeyboardLower() {
  char row0[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
  char row1[16] = {'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', ',', ' ', char(2), char(3), char(0)};
  for (int i = 0; i < 16; i++) {
    keyboard[0][0][i] = row0[i];
    keyboard[0][1][i] = row1[i];
  }
}

// Fill for uppercase
void fillKeyboardUpper() {
  char row0[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
  char row1[16] = {'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '?', '!', ' ', char(2), char(3), char(1)};
  for (int i = 0; i < 16; i++) {
    keyboard[1][0][i] = row0[i];
    keyboard[1][1][i] = row1[i];
  }
}

// Function to change the position of the cursor based on the input provided 
// by the joystick
void placeCursor() {
  currentMillis = millis();
  // Check if it's time to update
  if (currentMillis - lastChange >= cursorInterval) {
    // Read the analog values from the joystick's x and y pins
    int xVal = analogRead(xPin);
    int yVal = analogRead(yPin);
    // Update cursor x position making sure it is within bounds
    if (yVal >= 768) { // right
      col = min(15, col + 1);
    }
    else if (yVal <= 256) { // left
      col = max(0, col - 1);
    }
    // Update cursor y position making sure it is within bounds
    if (xVal >= 768) { // down
      row = min(1, row + 1); 
    }
    else if (xVal <= 256) { // up
      row = max(0, row - 1);
    }
    lcdkey.setCursor(col, row);
    lastChange = currentMillis;
  }
}

// Function to check the state of the joystick button/switch
void checkButton() {
  // Check if the debounce time has passed
  if ((millis() - lastDebounceTime) >= debounceDelay) {
    // Read the state of the button
    int reading = digitalRead(buttonPin);
    // Check if the button state has changed
    if (reading != lastButtonState) {
      // Reset the debounce timer
      lastDebounceTime = millis();
      // If the button state has changed
      if (reading == LOW) {
        lastButtonState = LOW;
        buttonPressed = true;
      }
      else {
        lastButtonState = HIGH;
      }
    } 
  }
}

// Function to update the string text
void joyButtonEvent(int keyType) {
  // If the text lcd screen is full don't do anything to the string
  if (rowText == 1 && colText > 15) {
    return;
  }
  // Backspace is pressed
  if (text && row == 1 && col == 13) {
    text.remove(text.length()-1);
  }
  // Enter is pressed
  else if (row == 1 && col == 14) {
    // Just reset the string for now
    text = "";
  }
  // Switch to upper/lowercase
  else if (row == 1 && col == 15) {
    keyboardType = 1 - keyboardType;
    updateKeyScreen(keyboardType);
  }
  // A character is added to the string
  else {
    text += keyboard[keyType][row][col];
  }
  // Print the current string in serial
  Serial.println(text);
}


// Update the text lcd screen
void updateTextScreen(int keyType) {
  // If the keyboard change key is pressed or if the screen is full
  if (row == 1 && col == 15) {
    return;
  }
  // Enter is pressed
  if (row == 1 && col == 14) {
    lcdtext.clear();
    rowText = 0;
    colText = 0;
  }
  // Backspace is pressed
  else if (row == 1 && col == 13) {
    colText--;
    // Handle case when the text cursor is in 0th col of row 0
    if (rowText ==  0 && colText == -1) {
      colText = 0;
    }
     // Handle case when the cursor has moved to row 1
    else if (rowText == 1 && colText == -1) {
      rowText = 0;
      colText = 15;
    }
    lcdtext.setCursor(colText, rowText);
    // Replace last character with space and move the cursor
    lcdtext.print(" ");
    lcdtext.setCursor(colText, rowText);
  }
  // A character is added
  else {
    // Print new character
    lcdtext.print(keyboard[keyType][row][col]);
    colText++;
    // Handle case when cursor is in 15th col of row 0
    if (rowText ==  0 && colText == 16) {
      rowText = 1;
      colText = 0;
    }
    // Handle case when screen if full
    else if (rowText ==  1 && colText == 17) {
      colText -= 1; // colText doesn't change
    }
    // Move cursor to the next position
    lcdtext.setCursor(colText, rowText);
  }
}

// Function to update the keyboard screen to lower/uppercase version
void updateKeyScreen(int keyType) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 16; j++) {
      lcdkey.setCursor(j, i);
      lcdkey.write(keyboard[keyType][i][j]);
    }
  }
}


