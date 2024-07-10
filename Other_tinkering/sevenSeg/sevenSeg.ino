// Check the 5161AS datasheet for more info on how the display operates


// Define a matrix with the values of each segment for each
// digit 0 to 9 (a value of 0 means LOW, 1 means HIGH)
int digits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1}
};


void setup() {
  pinMode(2, OUTPUT); // A segment
  pinMode(3, OUTPUT); // B segment
  pinMode(4, OUTPUT); // C segment
  pinMode(5, OUTPUT); // D segment
  pinMode(6, OUTPUT); // E segment
  pinMode(7, OUTPUT); // F segment
  pinMode(8, OUTPUT); // G segment
}

void loop() {
  // Display a digit every sec 
  for (int i = 0; i < 10; i++) {
    displayDigit(i);
    delay(1000);
  }
}

// Function that displays a digit
// on the seven segment display
void displayDigit(int d) {
  for (int i = 0; i < 7; i++) {
    if (digits[d][i] == 0) {
      digitalWrite(i+2, LOW);
    }
    else {
      digitalWrite(i+2, HIGH);
    }
  }
}



