/*
For now, it is up to the user to make sure that the settings set in step_signal.h to match the DRV8711_Tuning.ino sketch.
*/

#ifndef step_signal_h
#define step_signal_h

#include <Arduino.h>

// Pin definitions
const byte M1_StepSignalPin = 43;
// const byte M2_StepSignalPin = 42;

// Declare variables
extern unsigned long half_step_delay; // Units are milliseconds.
// bool useMicroseconds = true;
// unsigned int microstep_mode; // Valid modes of operation are full steps (1), half-steps (2), quarter-steps (4), 1/8 steps (8), 1/16 steps (16), 1/32 steps (32), 1/64 steps, (64), 1/128 steps (128), and 1/256 steps (256).
// const byte number_of_full_steps = 200; // The motor has 200 full steps to make 1 rotation. Each full step rotates the shaft by 1.8 degrees.
extern unsigned int number_of_steps_for_one_rotation; // Number of steps that it will take to make 1 revolution.
// unsigned int whatSpeed = 1; //Set speed of motor in rpm.

// Function declarations
void setupStepSignalFrequency(); 
void takeStep();

#endif