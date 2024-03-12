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
volatile byte state_of_M1_TOP_LIMIT_SWITCH = 0;
volatile byte state_of_M1_BOTTOM_LIMIT_SWITCH = 0;
volatile byte lastButtonState = HIGH;
volatile byte button_state;
volatile byte reading;
// Use when ready to test M2
// volatile byte state_of_M2_TOP_LIMIT_SWITCH = 0;
// volatile byte state_of_M2_BOTTOM_LIMIT_SWITCH = 0;


// Initialize Variables
const uint8_t rotations_per_button_press = 2; // Set how many rotations you want a button press to go here.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 0;    // the debounce time; increase if the output flickers



// Interrupt service routines
void emergencyStop()
{
  // EMERGENCY_BUTTON has been pressed.
  disableMotor();
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
  attachPCINT(digitalPinToPCINT(M1_TOP_LIMIT_SWITCH), topLimitSwitchTriggered, FALLING);
  attachPCINT(digitalPinToPCINT(M1_BOTTOM_LIMIT_SWITCH), bottomLimitSwitchTriggered, FALLING);
  // To-do: Add the remaing input buttons ISRs.
  delay(1000);
}

void checkButtonState()
{
  reading = digitalRead(ENABLE_BUTTON);

  if (reading != lastButtonState){lastDebounceTime = millis();}
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (reading != button_state) {
      button_state = reading;
      if (button_state == LOW){
        changeMotorState();
      }
    }
  }


  if(!digitalRead(M1_CW_BUTTON))
  {
    digitalWrite(M1_DirectionPin, HIGH);
    Timer1.pwm(12,0);
    uint32_t now1 = millis();
    while (((now1 + 1000U) > millis()) && (digitalRead(M1_TOP_LIMIT_SWITCH)))
    {
      Timer1.pwm(12, 512);
    }
    Timer1.pwm(12,0);
  }
  if(!digitalRead(M1_CCW_BUTTON))
  {
    digitalWrite(M1_DirectionPin, LOW);
    Timer1.pwm(12,0);
    uint32_t now2 = millis();
    while (((now2 + 1000U) > millis()) && (digitalRead(M1_BOTTOM_LIMIT_SWITCH)))
    {
      Timer1.pwm(12, 512);
    }
    Timer1.pwm(12, 0);
  }
}