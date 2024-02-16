// MotorController.cpp
#include "button_input.h"
#include <drv8711.h>

#define stepPin 10 //For now, this declaration can live in the buttons files.

// Global variables
uint8_t button_state = 1;
unsigned int PWMfreq = 200;
unsigned int step_delay = 1000000 / PWMfreq; // Result in microseconds
long int buttonReadTime = 0;
const int buttonReadDelay = 25; // Delay for 25 ms


void setupButtons() {
  pinMode(CW_button, INPUT_PULLUP);
  pinMode(CCW_button, INPUT_PULLUP);
  pinMode(EMERG_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EMERG_BUTTON), emerg_stop, LOW);
  pinMode(Enable_button, INPUT_PULLUP);
  pinMode(stepPin, OUTPUT);
  Timer1.initialize(step_delay);  // 5000us = 200Hz
}

void loopForButtonInput() {
  if (!digitalRead(Enable_button)) {
    changeState();
  }

  if (millis() > buttonReadTime + buttonReadDelay) {
    if (!digitalRead(CW_button) != !digitalRead(CCW_button)) {
      Timer1.pwm(10, 512);
    } else {
      Timer1.pwm(10, 0);
    }
    buttonReadTime = millis();
  }
}

void selectFrequency(int value) {
  PWMfreq = value;
  step_delay = 1000000 / PWMfreq;
  Timer1.initialize(step_delay);
}

