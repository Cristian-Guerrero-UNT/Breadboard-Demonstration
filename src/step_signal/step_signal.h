/*
For now, it is up to the user to make sure that the settings set in step_signal.h to match the DRV8711_Tuning.ino sketch.
*/

#ifndef button_input_interrupts_h
#define button_input_interrupts_h

#include <Arduino.h>

// Pin definitions
const byte stepSignalPin = 8;
const byte directionPin = 9;

// Define variables
unsigned long half_step_delay; // Units are milliseconds.
// bool useMicroseconds = true;
unsigned int microstep_mode = 1; // Valid modes of operation are full steps (1), half-steps (2), quarter-steps (4), 1/8 steps (8), 1/16 steps (16), 1/32 steps (32), 1/64 steps, (64), 1/128 steps (128), and 1/256 steps (256).
const byte number_of_full_steps = 200; // The motor has 200 full steps to make 1 rotation. Each full step rotates the shaft by 1.8 degrees.
unsinged int number_of_steps_for_one_rotation = microstep_mode * number_of_full_steps; // Number of steps that it will take to make 1 revolution.
unsigned int whatSpeed = 1; //Set speed of motor in rpm.

// Function declarations
void setupStepSignalFrequency(); 
void takeStep();

#endif