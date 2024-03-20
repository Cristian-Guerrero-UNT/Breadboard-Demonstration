/*
This file has the majority of the pin mappings

This uses the default SPI pins of the Mega2560.
CIPO - D50
COPI - D51
SCK - D52

*/

#include "button_input.h"

// Pin definitions
// User Input Buttons
const byte EMERGENCY_BUTTON = 21;
const byte RESET_ALL_MOTORS_BUTTON = 2;
const byte SM1_CW_BUTTON = 62;
const byte SM1_CCW_BUTTON = 63;
const byte SM2_CW_BUTTON = 64;
const byte SM2_CCW_BUTTON = 65;
const byte ACT_UP_BUTTON = 66;
const byte ACT_DOWN_BUTTON = 67;
// DRV8711 specific Pins
const byte SM1_DirectionPin = 45;
const byte SM1_StepPin = 43;
const byte SM2_DirectionPin = 44;
const byte SM2_StepPin = 42;
const byte ACT_AIN1 = 41;
const byte ACT_AIN2 = 39;
// Limit Switches
const byte SM1_TOP_LIMIT_SWITCH = 10;
const byte SM1_BOTTOM_LIMIT_SWITCH = 11;
const byte SM2_TOP_LIMIT_SWITCH = 12;
const byte SM2_BOTTOM_LIMIT_SWITCH = 13;
// Fault Pins
const byte SM1_FaultPin = 20;
const byte SM2_FaultPin = 19;
const byte ACT_FaultPin = 18;


// Initialize Variables
const byte rotations_per_button_press = 2; // Set how many rotations you want a button press to go here.
unsigned long time_for_rotation = rotations_per_button_press * 1000UL; // The units need to be in milliseconds to work properly with the PWM initialization.
unsigned long lastDebounceTime = 0;        // the last time the output pin was toggled
unsigned long debounceDelay = 0;           // the debounce time; increase if the output flickers

// Interrupt service routines
void emergencyStop()
{
  // EMERGENCY_BUTTON has been pressed.
  disableMotor();
}


void setupButtons()
{
  // Set pins as output.
  pinMode(SM1_DirectionPin, OUTPUT);
  // pinMode(SM2_DirectionPin, OUTPUT);
  // Set button pins as input.
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(SM1_CW_BUTTON, INPUT_PULLUP);
  pinMode(SM1_CCW_BUTTON, INPUT_PULLUP);
  // pinMode(M2_CW_BUTTON, INPUT_PULLUP);
  // pinMode(M2_CCW_BUTTON, INPUT_PULLUP);
  // Set limit switches as input.
  pinMode(SM1_TOP_LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(SM1_BOTTOM_LIMIT_SWITCH, INPUT_PULLUP);
  // pinMode(M2_TOP_LIMIT_SWITCH, INPUT_PULLUP);
  // pinMode(M2_BOTTOM_LIMIT_SWITCH, INPUT_PULLUP);

  // Initialize the Timer One PWM generation.
  Timer1.initialize(5000); // Creates a 200 Hz PWM signal. Each cycle corresponds to a step at the DRV8711.
  Timer1.pwm(SM1_StepPin, 0);
  Timer1.pwm(SM2_StepPin, 0);

  // Attach interrupts to pins.
  // Arduino Interrupts allow for LOW, HIGH, CHANGE, RISING, AND FALLING as valid modes.
  // The PinChangeInterrupt library has RISING, FALLING, and CHANGE as valid modes of operation.
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergencyStop, FALLING);
  // TODO - Define the FaultPinISRs for the three fault pins.
  // attachInterrupt(digitalPinToInterrupt(SM1_FaultPin), FaultPinISR, FALLING);
  // attachInterrupt(digitalPinToInterrupt(SM2_FaultPin), FaultPinISR, FALLING);
  // attachInterrupt(digitalPinToInterrupt(ACT_FaultPin), FaultPinISR, FALLING);
  delay(1000);
}

void actionForButton()
{
  // TODO - decide if these variables should be local or global.
  bool lastButtonState = HIGH; // the previous reading from the input pin
  bool buttonState; // the current reading from the input pin
  bool reading = digitalRead(ENABLE_BUTTON);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != button_state)
    {
      button_state = reading;
      if (button_state == LOW)
      {
        enableMotor();
      }
    }
    // Save the reading. Next time through the loop, it'll be the lastButtonState.
    lastButtonState = reading;
  }

  // Check to see if the SM1_CW_BUTTON was pressed.
  if (!digitalRead(SM1_CW_BUTTON))
  {
    digitalWrite(SM1_DirectionPin, HIGH);
    unsigned long now = millis();
    while (((now + time_for_rotation) > millis()) && (digitalRead(SM1_TOP_LIMIT_SWITCH)))
    {
      Timer1.pwm(SM1_StepPin, 512);
    }
    Timer1.pwm(SM1_StepPin, 0);
  }

  // Check to see if SM1_CCW_BUTTON was pressed.
  if (!digitalRead(SM1_CCW_BUTTON))
  {
    digitalWrite(SM1_DirectionPin, LOW);
    unsigned long now  = millis();
    while (((now + time_for_rotation) > millis()) && (digitalRead(SM1_BOTTOM_LIMIT_SWITCH)))
    {
      Timer1.pwm(SM1_StepPin, 512);
    }
    Timer1.pwm(SM1_StepPin, 0);
  }

  // Check to see if the SM2_CW_BUTTON was pressed.
  if (!digitalRead(SM2_CW_BUTTON))
  {
    digitalWrite(SM2_DirectionPin, HIGH);
    unsigned long now  = millis();
    while (((now + time_for_rotation) > millis()) && (digitalRead(SM2_TOP_LIMIT_SWITCH)))
    {
      Timer1.pwm(SM2_StepPin, 512);
    }
    Timer1.pwm(SM2_StepPin, 0);
  }

  // Check to see if SM2_CCW_BUTTON was pressed.
  if (!digitalRead(SM2_CCW_BUTTON))
  {
    digitalWrite(SM2_DirectionPin, LOW);
    unsigned long now  = millis();
    while (((now + time_for_rotation) > millis()) && (digitalRead(SM2_BOTTOM_LIMIT_SWITCH)))
    {
      Timer1.pwm(SM2_StepPin, 512);
    }
    Timer1.pwm(SM2_StepPin, 0);
  }

  // TODO - Add Actuator code.
  // ACTUATOR code here
}