#include "button_input_interrupts.h"
#include "step_signal.h"

// Define the actions for each button
void actionForButton(byte buttonPin) {
  switch (buttonPin) {
    case EMERGENCY_BUTTON:
      // Drive Stepper Motor Clockwise
      // Red LED On
      Serial.println("Red LED On");
      digitalWrite(EMERGENCY_LED, HIGH);
      delay(1000);
      digitalWrite(EMERGENCY_LED, LOW);
      state_of_EMERGENCY_BUTTON = 0;
      break;
    case ENABLE_BUTTON:
      // Enable stepper motor
      // Green LED On
      Serial.println("Green LED On");
      digitalWrite(ENABLE_LED, HIGH);
      delay(1000);
      digitalWrite(ENABLE_LED, LOW);
      state_of_ENABLE_BUTTON = 0;
      break;


    default:
      // If we enter here message Error
      Serial.println("There has been an error within the switch statement.");
      break;
  }
}

// This function should execute 2 rotations of the stepper motor. 
// To-do:
// The 400 should be replaced by the number_of_steps_for_rotation variable.
void executeRotation(byte buttonPin) {
    unsigned int counter = 0;
    byte button = buttonPin;

    // Take steps in the direction called for by the user.
    if (button == CW_BUTTON) {
        directionPin = HIGH;
        while(counter < 400 && state_of_TOP_LIMIT_SWITCH == 0) {
            takeStep();
            counter++;
        }
    }
    else {  // CCW button was pressed.
        directionPin = LOW;
        while(counter < 400 && state_of_BOTTOM_LIMIT_SWITCH == 0) {
            takeStep();
            counter++;
        }
    }

    // 
    if (state_of_TOP_LIMIT_SWITCH != 0) {
        state_of_TOP_LIMIT_SWITCH = 0;
    }
    if (state_of_BOTTOM_LIMIT_SWITCH != 0) {
        state_of_BOTTOM_LIMIT_SWITCH = 0;
    }
}

// Interrupt service routines
void emergency_stop() {
  // Red LED On
  state_of_EMERGENCY_BUTTON = EMERGENCY_BUTTON;
}

// Interrupt service routines
void pinChangeInterrupt1() {
  // Green LED On
  state_of_ENABLE_BUTTON = ENABLE_BUTTON;
}

void pinChangeInterrupt2() {
  // Yellow1 LED On
  state_of_CW_BUTTON = CW_BUTTON;
}

void pinChangeInterrupt3() {
  // Yellow2 LED On
  state_of_CCW_BUTTON = CCW_BUTTON;
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

