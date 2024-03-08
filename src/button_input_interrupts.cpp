#include "button_input_interrupts.h"

// Pin definitions
const byte M1_DirectionPin = 45;
// const byte M2_DirectionPin = 44;
const byte EMERGENCY_BUTTON = 21;
const byte ENABLE_BUTTON = 20;
const byte M1_CW_BUTTON = 13;
const byte M1_CCW_BUTTON = 12;
// To-do: Assign pins on the MEGA to these buttons.
// const byte M2_CW_BUTTON = ;
// const byte M2_CCW_BUTTON = ;
// Limit switches
const byte M1_TOP_LIMIT_SWITCH = 11;
const byte M1_BOTTOM_LIMIT_SWITCH = 10;
// To-do: Assign pins on the MEGA to these buttons.
// const byte M2_TOP_LIMIT_SWITCH = ;
// const byte M2_BOTTOM_LIMIT_SWITCH = ;

// Initialize volatile variables
volatile byte state_of_EMERGENCY_BUTTON = 0;
volatile byte state_of_ENABLE_BUTTON = 0;
volatile byte state_of_M1_CW_BUTTON = 0;
volatile byte state_of_M1_CCW_BUTTON = 0;
volatile byte state_of_M1_TOP_LIMIT_SWITCH = 0;
volatile byte state_of_M1_BOTTOM_LIMIT_SWITCH = 0;
// Use when ready to test M2
// volatile byte state_of_M2_CW_BUTTON = 0;
// volatile byte state_of_M2_CCW_BUTTON = 0;
// volatile byte state_of_M2_TOP_LIMIT_SWITCH = 0;
// volatile byte state_of_M2_BOTTOM_LIMIT_SWITCH = 0;


// Initialize Variables
const byte rotations_per_button_press = 2; // Set how many rotations you want a button press to go here.
unsigned long steps_per_button_press = rotations_per_button_press * number_of_steps_for_one_rotation;// The max value this could be is 256 step_mode * 200 full steps per rotation * 60 rotations from one end of the track to the other end = 3,072,000 1/256 steps.


// To-do: Decide and select appropriate outcome for emergency stop and enable.
// Define the actions for each button
void actionForButton(byte buttonPin)
{
  unsigned int counter = 0; // For CW and CCW rotation.

  switch (buttonPin)
  {
  case EMERGENCY_BUTTON:
    // Disable stepper motor
    disableMotor();
    state_of_EMERGENCY_BUTTON = 0;
    break;

  case ENABLE_BUTTON:
    // Enable stepper motor
    changeMotorState();
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
    state_of_M1_CW_BUTTON = 0;
    // Clear the limit switch flag after a successful rotation. The carriage should no longer be on the limit switch.
    if (state_of_M1_BOTTOM_LIMIT_SWITCH != 0)
    {
      state_of_M1_BOTTOM_LIMIT_SWITCH = 0;
    }
    break;

  case M1_CCW_BUTTON:
    // Rotate the stepper motor in CCW direction
    digitalWrite(M1_DirectionPin, LOW);
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
  // EMERGENCY_BUTTON has been pressed.
  state_of_EMERGENCY_BUTTON = EMERGENCY_BUTTON;
}

void enableMotors()
{
  // ENABLE_BUTTON has been pressed.
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
  attachInterrupt(digitalPinToInterrupt(ENABLE_BUTTON), enableMotors, FALLING);
  attachPCINT(digitalPinToPCINT(M1_CW_BUTTON), requestCWRotation, FALLING);
  attachPCINT(digitalPinToPCINT(M1_CCW_BUTTON), requestCCWRotation, FALLING);
  attachPCINT(digitalPinToPCINT(M1_TOP_LIMIT_SWITCH), topLimitSwitchTriggered, FALLING);
  attachPCINT(digitalPinToPCINT(M1_BOTTOM_LIMIT_SWITCH), bottomLimitSwitchTriggered, FALLING);
  // To-do: Add the remaing input buttons ISRs.
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