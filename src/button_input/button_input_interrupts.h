
#ifndef button_input_interrupts_h
#define button_input_interrupts_h

#include <Arduino.h>
#include <PinChangeInterrupt.h>

// Pin definitions
#define EMERGENCY_BUTTON 2
#define ENABLE_BUTTON 7
#define CW_BUTTON 7
#define CCW_BUTTON 5

// Function declarations
void setupButtons();
void loopForButtonInput();
void changeState();
void selectFrequency(int value);
void emerg_stop();

#endif
