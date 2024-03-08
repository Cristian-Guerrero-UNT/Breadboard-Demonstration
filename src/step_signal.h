/*
For now, it is up to the user to make sure that the settings set in step_signal.h to match the DRV8711_Tuning.ino sketch.
*/

#ifndef step_signal_h
#define step_signal_h

#include <Arduino.h>

// Declare variables
extern unsigned int number_of_steps_for_one_rotation; // Number of steps that it will take to make 1 revolution.

// Function declarations
void setupStepSignalFrequency(); 
void takeStep();

#endif