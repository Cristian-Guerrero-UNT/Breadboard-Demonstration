
#ifndef button_input_interrupts_h
#define button_input_interrupts_h

#include <Arduino.h>
#include <PinChangeInterrupt.h>
#include "step_signal.h"

// Pin definitions
const byte EMERGENCY_BUTTON = 2;
const byte ENABLE_BUTTON = 7;
const byte CW_BUTTON = 6;
const byte CCW_BUTTON = 5;
// Limit switches
const byte TOP_LIMIT_SWITCH = 4;
const byte BOTTOM_LIMIT_SWITCH = 3;

// Define volatile variables
volatile byte state_of_EMERGENCY_BUTTON = 0;
volatile byte state_of_ENABLE_BUTTON = 0;
volatile byte state_of_CW_BUTTON = 0;
volatile byte state_of_CCW_BUTTON = 0;
volatile byte state_of_TOP_LIMIT_SWITCH = 0;
volatile byte state_of_BOTTOM_LIMIT_SWITCH = 0;

// Function declarations
void setupButtons();
void actionForButton(byte buttonPin);
void executeRotation(byte buttonPin)
void emergency_stop();
void pinChangeInterrupt1();
void pinChangeInterrupt2();
void pinChangeInterrupt3();

#endif
