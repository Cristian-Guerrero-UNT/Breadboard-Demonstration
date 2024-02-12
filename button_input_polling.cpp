// Arduino Uno Rev3 has pins 2 and 3 for interrupts.
// Arduino Mega 2560 has pins 2, 3, 18, 19, 20, and 21 available for interrupts.

const int buttonPin = 2;
bool buttonPressed = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Check the state of the button
  bool buttonState = digitalRead(buttonPin);

  // Check if the button state has changed
  if (buttonState != buttonPressed) {
    buttonPressed = buttonState;

    // Execute your code when the button state changes
    if (buttonPressed) {
      // Your code to execute when the button is pressed
    } else {
      // Your code to execute when the button is released
    }
  }
}
