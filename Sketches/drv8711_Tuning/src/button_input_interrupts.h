/*
Ensure that none of the pins set here happen to be RX or TX pins.
This has caused issues uploading sketches in the past.
*/

#ifndef button_input_interrupts_h
#define button_input_interrupts_h

#include <Arduino.h>
#include <PinChangeInterrupt.h>
#include "step_signal.h"
#include <TimerOne.h>

// Function declarations
void setupButtons();
void checkButtonState();
void enableMotor(); // Defined in the drv8711_Tuning.ino sketch.
void disableMotor(); // Defined in the drv8711_Tuning.ino sketch.

#endif
