#include "button_input_interrupts.h"


// Define the pins for the buttons
const int buttonPins[] = {2, 3, 4, 5, 6, 7};
const int numButtons = 6;

// Define the actions for each button
void actionForButton(int buttonPin) {
  switch (buttonPin) {
    case 2:
      // Action for button connected to pin 2
      break;
    case 3:
      // Action for button connected to pin 3
      break;
    case 4:
      // Action for button connected to pin 4
      break;
    case 5:
      // Action for button connected to pin 5
      break;
    case 6:
      // Action for button connected to pin 6
      break;
    case 7:
      // Action for button connected to pin 7
      break;
    default:
      // Default action
      break;
  }
}

// Interrupt service routine
void pinChangeInterrupt() {
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      actionForButton(buttonPins[i]);
    }
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set button pins as input
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Attach pin change interrupt
  attachInterrupt(digitalPinToInterrupt(buttonPins[0]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[1]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[2]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[3]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[4]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[5]), pinChangeInterrupt, CHANGE);
}

void loop() {
  // Main program loop
  // You can add other tasks here if needed
}
