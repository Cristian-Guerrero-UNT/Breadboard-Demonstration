#include "button_input_interrupts.h"


// Define the actions for each button
void actionForButton(int buttonPin) {
  switch (buttonPin) {
    case CW_BUTTON:
      // Drive Stepper Motor Clockwise
      // Turn an LED on to test

      break;
    case CCW_BUTTON:
      // // Drive Stepper Motor Counter-Clockwise
      // Turn an LED on to test
      
      break;
    default:
      // If we enter here message Error
      println("There has been an error with the switch statement.");
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

void setupButtons() {
  // Set button pins as input
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(CW_BUTTON, INPUT_PULLUP);
  pinMode(CCW_BUTTON, INPUT_PULLUP);

  // Attach pin change interrupt
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emerg_stop, LOW);
  attachPCINT(digitalPinToPCINT(ENABLE_BUTTON), pinChangeInterrupt, CHANGE);
  attachPCINT(digitalPinToPCINT(CW_BUTTON), pinChangeInterrupt, CHANGE);
  attachPCINT(digitalPinToPCINT(CCW_BUTTON), pinChangeInterrupt, CHANGE);


}

