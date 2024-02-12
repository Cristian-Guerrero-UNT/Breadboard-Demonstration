// Arduino Uno Rev3 has pins 2 and 3 for interrupts.
// Arduino Mega 2560 has pins 2, 3, 18, 19, 20, and 21 available for interrupts.

const int buttonPin = 2;
volatile bool buttonPressed = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
}

void loop() {
  if (buttonPressed) {
    // Your code to execute when the button is pressed
    // This code will run continuously while the button is held down
  }
}

void handleButtonPress() {
  buttonPressed = !digitalRead(buttonPin); // Update buttonPressed flag
}
