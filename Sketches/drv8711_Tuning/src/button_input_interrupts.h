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

extern unsigned int number_of_steps_for_one_rotation; // Number of steps that it will take to make 1 revolution.

// Function declarations
void setupButtons();
void checkButtonState();
void changeMotorState(); // Defined in the drv8711_Tuning.ino sketch.
void disableMotor(); // Defined in the drv8711_Tuning.ino sketch.

#endif
