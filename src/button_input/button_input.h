// button_input.h
#ifndef button_input_h
#define button_input_h

#include <Arduino.h>
#include <TimerOne.h>

// Pin definitions
#define EMERG_BUTTON 2
#define Enable_button 0
#define CW_button 1
#define CCW_button 8

// Function declarations
void setupButtons();
void loopForButtonInput();
void changeState();
void selectFrequency(int value);
void emerg_stop();

#endif
