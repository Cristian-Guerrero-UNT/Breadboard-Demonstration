
#ifndef button_input_interrupts_h
#define button_input_interrupts_h

#include <Arduino.h>
#include <PinChangeInterrupt.h>
#include "step_signal.h"

// Pin definitions
const byte directionPin = 9;
const byte ENABLE_BUTTON = 7;
const byte CW_BUTTON = 6;
const byte CCW_BUTTON = 5;
const byte EMERGENCY_BUTTON = 2;
// Limit switches
const byte TOP_LIMIT_SWITCH = 1;
const byte BOTTOM_LIMIT_SWITCH = 0;

// Define volatile variables
volatile byte state_of_EMERGENCY_BUTTON = 0;
volatile byte state_of_ENABLE_BUTTON = 0;
volatile byte state_of_CW_BUTTON = 0;
volatile byte state_of_CCW_BUTTON = 0;
volatile byte state_of_TOP_LIMIT_SWITCH = 0;
volatile byte state_of_BOTTOM_LIMIT_SWITCH = 0;
// Variables
byte rotations_per_button_press = 2;
unsigned long steps_per_button_press = rotations_per_button_press * number_of_steps_for_one_rotation;// The max value this could be is 256 step_mode * 200 full steps per rotation * 60 rotations from one end of the track to the other end = 3,072,000 1/256 steps.

// Function declarations
void setupButtons();
void actionForButton(byte buttonPin);
void executeRotation(byte buttonPin);
void checkButtonState();
void emergency_stop();
// Rename with more appropriate names after debugging logic.
void pinChangeInterrupt1();
void pinChangeInterrupt2();
void pinChangeInterrupt3();
void pinChangeInterrupt4();
void pinChangeInterrupt5();

#endif
