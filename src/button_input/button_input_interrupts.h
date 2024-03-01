/*
Ensure that none of the pins set here happen to be RX or TX pins.
This has caused issues uploading sketches in the past.
*/

#ifndef button_input_interrupts_h
#define button_input_interrupts_h

#include <Arduino.h>
#include <PinChangeInterrupt.h>
#include "step_signal.h"

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

// Define volatile variables
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

// Variables
bool motorState = false; // Variable for button press to enable or disable motor
byte rotations_per_button_press = 2;
unsigned long steps_per_button_press = rotations_per_button_press * number_of_steps_for_one_rotation;// The max value this could be is 256 step_mode * 200 full steps per rotation * 60 rotations from one end of the track to the other end = 3,072,000 1/256 steps.

// Function declarations
void setupButtons();
void actionForButton(byte buttonPin);
void checkButtonState();
// Interrupt Service Routines
void emergencyStop();
void enableMotors();
void requestCWRotation();
void requestCCWRotation();
void topLimitSwitchTriggered();
void bottomLimitSwitchTriggered();

#endif
