#include "button_input_interrupts.h"
#include "step_signal.h"

// To-do: Decide and select appropriate outcome for emergency stop and enable.
// Define the actions for each button
void actionForButton(byte buttonPin)
{
  unsigned int counter = 0;

  switch (buttonPin)
  {
  case EMERGENCY_BUTTON:
    // Disable stepper motor
    Axis[currentAxis].disable();
    state_of_EMERGENCY_BUTTON = 0;
    break;
  case ENABLE_BUTTON:
    // Enable stepper motor
    motorState = !motorState;
    setMotor(motorState);
    state_of_ENABLE_BUTTON = 0;
    break;
  case M1_CW_BUTTON:
    // Rotate the stepper motor in CW direction
    digitalWrite(M1_DirectionPin, HIGH);
    while ((counter < steps_per_button_press) && (state_of_M1_TOP_LIMIT_SWITCH == 0))
    {
      takeStep();
      counter++;
    }
    state_of_M1_CW_BUTTON = 0; //
    // Clear the limit switch flag after a successful rotation. The carriage should no longer be on the limit switch.
    if (state_of_M1_BOTTOM_LIMIT_SWITCH != 0)
    {
      state_of_M1_BOTTOM_LIMIT_SWITCH = 0;
    }
    break;
  case M1_CCW_BUTTON:
    // Rotate the stepper motor in CCW direction
    digitalWrite(CCW_LED, HIGH);
    while ((counter < steps_per_button_press) && (state_of_M1_BOTTOM_LIMIT_SWITCH == 0))
    {
      takeStep();
      counter++;
    }
    state_of_M1_CCW_BUTTON = 0;
    // Clear the limit switch flag after a successful rotation. The carriage should no longer be on the limit switch.
    if (state_of_M1_TOP_LIMIT_SWITCH != 0)
    {
      state_of_M1_TOP_LIMIT_SWITCH = 0;
    }
    break;
  default: // To-do: create a default case that is more relavent for when the system is operating in production environment.
    // If we enter here message Error
    Serial.println("There has been an error within the switch statement.");
    break;
  }
}



// Interrupt service routines
void emergencyStop()
{
  // Red LED On
  state_of_EMERGENCY_BUTTON = EMERGENCY_BUTTON;
}

void enableMotors()
{
  // Green LED On
  state_of_ENABLE_BUTTON = ENABLE_BUTTON;
}

void requestCWRotation()
{
  // Check if the corresponding limit switch is NOT pressed down.
  // Set flag to enable rotation. Otherwise, ignore request.
  if (state_of_M1_TOP_LIMIT_SWITCH == 0)
  {
    state_of_M1_CW_BUTTON = M1_CW_BUTTON;
  }
}

void requestCCWRotation()
{
  // Check if the corresponding limit switch is NOT pressed down.
  // Set flag to enable rotation. Otherwise, ignore request.
  if (state_of_M1_BOTTOM_LIMIT_SWITCH == 0)
  {
    state_of_M1_CCW_BUTTON = M1_CCW_BUTTON;
  }
}

void topLimitSwitchTriggered()
{
  // TOP_LIMIT_SWITCH has been pressed down.
  state_of_M1_TOP_LIMIT_SWITCH = 1;
}

void bottomLimitSwitchTriggered()
{
  // BOTTOM_LIMIT_SWITCH has been pressed down.
  state_of_M1_BOTTOM_LIMIT_SWITCH = 1;
}

void setupButtons()
{
  // Set pins as output.
  pinMode(M1_DirectionPin, OUTPUT);
  // pinMode(M2_DirectionPin, OUTPUT);
  // Set button pins as input.
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(M1_CW_BUTTON, INPUT_PULLUP);
  pinMode(M1_CCW_BUTTON, INPUT_PULLUP);
  // pinMode(M2_CW_BUTTON, INPUT_PULLUP);
  // pinMode(M2_CCW_BUTTON, INPUT_PULLUP);
  // Set limit switches as input.
  pinMode(M1_TOP_LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(M1_BOTTOM_LIMIT_SWITCH, INPUT_PULLUP);
  // pinMode(M2_TOP_LIMIT_SWITCH, INPUT_PULLUP);
  // pinMode(M2_BOTTOM_LIMIT_SWITCH, INPUT_PULLUP);

  // Attach interrupts to pins.
  // Arduino Interrupts allow for LOW, HIGH, CHANGE, RISING, AND FALLING as valid modes.
  // The PinChangeInterrupt library has RISING, FALLING, and CHANGE as valid modes of operation.
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergencyStop, FALLING);
  attachPCINT(digitalPinToPCINT(ENABLE_BUTTON), enableMotors, FALLING);
  attachPCINT(digitalPinToPCINT(M1_CW_BUTTON), requestCWRotation, FALLING);
  attachPCINT(digitalPinToPCINT(M1_CCW_BUTTON), requestCCWRotation, FALLING);
  attachPCINT(digitalPinToPCINT(M1_TOP_LIMIT_SWITCH), topLimitSwitchTriggered, FALLING);
  attachPCINT(digitalPinToPCINT(M1_BOTTOM_LIMIT_SWITCH), bottomLimitSwitchTriggered, FALLING);
}

void checkButtonState()
{
  // Check to  see if any buttons were pressed.
  // If a button was pressed call the relevant function for output.
  if (state_of_EMERGENCY_BUTTON != 0)
  {
    actionForButton(state_of_EMERGENCY_BUTTON);
  }
  if (state_of_ENABLE_BUTTON != 0)
  {
    actionForButton(state_of_ENABLE_BUTTON);
  }
  if (state_of_M1_CW_BUTTON != 0)
  {
    actionForButton(state_of_M1_CW_BUTTON);
  }
  if (state_of_M1_CCW_BUTTON != 0)
  {
    actionForButton(state_of_M1_CCW_BUTTON);
  }
}