#include "button_input_interrupts.h"
#include "step_signal.h"

// To-do: Decide and select appropriate outcome for emergency stop and enable.
// Define the actions for each button
void actionForButton(byte buttonPin) {
  switch (buttonPin) {
    case EMERGENCY_BUTTON:
      // Stop stepper motor
      // Perform action and clear the flag.
      state_of_EMERGENCY_BUTTON = 0;
      break;
    case ENABLE_BUTTON:
      // Enable stepper motor
      // Perform action and clear the flag.
      state_of_ENABLE_BUTTON = 0;
      break;
    default:
      // If we enter here message Error
      Serial.println("There has been an error within the switch statement.");
      break;
  }
}

// This function should execute 2 rotations of the stepper motor.
void executeRotation(byte buttonPin) {
    unsigned int counter = 0;
    byte button = buttonPin;

    // Take steps in the CW direction.
    if (button == CW_BUTTON) {
        digitalWrite(directionPin, HIGH);
        while(counter < steps_per_button_press && state_of_TOP_LIMIT_SWITCH == 0) {
            takeStep();
            counter++;
        }
    }
    else {  // CCW button was pressed.
        digitalWrite(directionPin, LOW);
        while(counter < steps_per_button_press && state_of_BOTTOM_LIMIT_SWITCH == 0) {
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

void pinChangeInterrupt4()
{
  // TOP_LIMIT_SWITCH has been pressed down.
  state_of_TOP_LIMIT_SWITCH = 1;
}

void pinChangeInterrupt5()
{
  // BOTTOM_LIMIT_SWITCH has been pressed down.
  state_of_BOTTOM_LIMIT_SWITCH = 1;
}

void setupButtons() {
  // Set button pins as input
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(CW_BUTTON, INPUT_PULLUP);
  pinMode(CCW_BUTTON, INPUT_PULLUP);
  // Set limit switches as input.
  pinMode(TOP_LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(BOTTOM_LIMIT_SWITCH, INPUT_PULLUP);

  // Attach interrupts to pins.
  // Arduino Interrupts allow for LOW, HIGH, CHANGE, RISING, AND FALLING as valid modes.
  // The PinChangeInterrupt library has RISING, FALLING, and CHANGE as valid modes of operation.
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergency_stop, FALLING);
  attachPCINT(digitalPinToPCINT(ENABLE_BUTTON), pinChangeInterrupt1, FALLING);
  attachPCINT(digitalPinToPCINT(CW_BUTTON), pinChangeInterrupt2, FALLING);
  attachPCINT(digitalPinToPCINT(CCW_BUTTON), pinChangeInterrupt3, FALLING);
  attachPCINT(digitalPinToPCINT(TOP_LIMIT_SWITCH), pinChangeInterrupt4, FALLING);
  attachPCINT(digitalPinToPCINT(BOTTOM_LIMIT_SWITCH), pinChangeInterrupt5, FALLING);
}

void checkButtonState() {
  // Check to  see if any buttons were pressed.
  // If a button was pressed call the relevant function for output.
  if (state_of_EMERGENCY_BUTTON != 0)
  {
    actionForButton(state_of_EMERGENCY_BUTTON);
  }
  else if (state_of_ENABLE_BUTTON != 0)
  {
    actionForButton(state_of_ENABLE_BUTTON);
  }
  else if (state_of_CW_BUTTON != 0)
  {
    executeRotation(state_of_CW_BUTTON);
  }
  else if (state_of_CCW_BUTTON != 0)
  {
    executeRotation(state_of_CCW_BUTTON);
  }
}