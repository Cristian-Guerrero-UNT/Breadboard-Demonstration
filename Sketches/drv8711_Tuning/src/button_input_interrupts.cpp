#include "button_input_interrupts.h"
#include "step_signal.h"

// Pin definitions
const byte M1_DirectionPin = 45;
// const byte M2_DirectionPin = 44;
const byte EMERGENCY_BUTTON = 21;
const byte ENABLE_BUTTON = 2;
const byte M1_CW_BUTTON = 6;
const byte M1_CCW_BUTTON = 5;
// To-do: Assign pins on the MEGA to these buttons.
// const byte M2_CW_BUTTON = ;
// const byte M2_CCW_BUTTON = ;
// Limit switches
const byte M1_TOP_LIMIT_SWITCH = 10;
const byte M1_BOTTOM_LIMIT_SWITCH = 11;
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
volatile byte lastButtonState = HIGH;
volatile byte button_state;
// Use when ready to test M2
// volatile byte state_of_M2_CW_BUTTON = 0;
// volatile byte state_of_M2_CCW_BUTTON = 0;
// volatile byte state_of_M2_TOP_LIMIT_SWITCH = 0;
// volatile byte state_of_M2_BOTTOM_LIMIT_SWITCH = 0;


// Initialize Variables
const uint8_t rotations_per_button_press = 2; // Set how many rotations you want a button press to go here.
//unsigned long steps_per_button_press = rotations_per_button_press * number_of_steps_for_one_rotation;// The max value this could be is 256 step_mode * 200 full steps per rotation * 60 rotations from one end of the track to the other end = 3,072,000 1/256 steps.
unsigned long steps_per_button_press = 40000;
unsigned long int buttonReadTime = 0;
const unsigned short buttonReadDelay = 1000;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 0;    // the debounce time; increase if the output flickers

// To-do: Decide and select appropriate outcome for emergency stop and enable.
// Define the actions for each button
void actionForButton(byte buttonPin)
{
  unsigned int counter = 0; // For CW and CCW rotation.

  switch (buttonPin)
  {
  case EMERGENCY_BUTTON:
    // Disable stepper motor
    Serial.println("Inside emergency button routine.");
    disableMotor();
    state_of_EMERGENCY_BUTTON = 0;
    break;

  case ENABLE_BUTTON:
    // Enable stepper motor
    Serial.println("Inside ENABLE button routine.");
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
  disableMotor();
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
  pinMode(12, OUTPUT);
  Timer1.initialize(5000);
  Timer1.pwm(12, 0);

  // Attach interrupts to pins.
  // Arduino Interrupts allow for LOW, HIGH, CHANGE, RISING, AND FALLING as valid modes.
  // The PinChangeInterrupt library has RISING, FALLING, and CHANGE as valid modes of operation.
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergencyStop, FALLING);
  //attachInterrupt(digitalPinToInterrupt(ENABLE_BUTTON), enableMotors, FALLING);
  //attachPCINT(digitalPinToPCINT(M1_CW_BUTTON), requestCWRotation, FALLING);
  //attachPCINT(digitalPinToPCINT(M1_CCW_BUTTON), requestCCWRotation, FALLING);
  attachPCINT(digitalPinToPCINT(M1_TOP_LIMIT_SWITCH), topLimitSwitchTriggered, FALLING);
  attachPCINT(digitalPinToPCINT(M1_BOTTOM_LIMIT_SWITCH), bottomLimitSwitchTriggered, FALLING);
  // To-do: Add the remaing input buttons ISRs.
  delay(1000);
}

void checkButtonState()
{
  volatile byte reading = digitalRead(ENABLE_BUTTON);

  if (reading != lastButtonState){lastDebounceTime = millis();}
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (reading != button_state) {
      button_state = reading;
      if (button_state == LOW){
        changeMotorState();
        Serial.println("rotations_per_button_press: " + String(rotations_per_button_press));
  Serial.println("steps_per_button_press: " + String(steps_per_button_press));
  Serial.println("rotations_per_button_press: " + String(rotations_per_button_press));
  Serial.println("number_of_steps_for_one_rotation: " + String(number_of_steps_for_one_rotation));
      }
    }
  }

  //unsigned int counter1 = 0; // For CW and CCW rotation.
  unsigned int counter2 = 0; // For CW and CCW rotation.

  if(!digitalRead(M1_CW_BUTTON))
  {
    digitalWrite(M1_DirectionPin, HIGH);
    Timer1.pwm(12, 512);
    while ((millis() > buttonReadTime + buttonReadDelay) && (digitalRead(M1_TOP_LIMIT_SWITCH)))
    {
      //Serial.println("counter1: " + String(counter1));
    }
    buttonReadTime = millis();
    Timer1.pwm(12,0);
    // if(state_of_M1_TOP_LIMIT_SWITCH != 0)
    // {
    //   state_of_M1_TOP_LIMIT_SWITCH = 0;
    // }
  }
  if(!digitalRead(M1_CCW_BUTTON))
  {
    digitalWrite(M1_DirectionPin, LOW);
    while ((counter2 < steps_per_button_press) && (state_of_M1_BOTTOM_LIMIT_SWITCH == 0))
    {
      Timer1.pwm(12, 512);
      counter2++;
    }
    Timer1.pwm(12, 0);
    // if(state_of_M1_BOTTOM_LIMIT_SWITCH != 0)
    // {
    //   state_of_M1_BOTTOM_LIMIT_SWITCH = 0;
    // }
  }
}