/*
Create pulse and direction signals as per the PUL/DIR mode described in the DM860T manual.

Note: delayMicroseconds() can only reliably produce a delay up to 16,383 microseconds.
https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
For delays greater than 16,383 microseconds (16.383 milliseconds) use delay() instead.

For now, it is up to the user to make sure that the settings set in step_signal.h to match the DRV8711_Tuning.ino sketch.

To-do: Since the takeStep() function uses delay(), it may cause trouble with the use of other interrupts in our program.
If so, an appropriate delay may have to be implemented with millis() instead.
*/

#include "step_signal.h"

// At setup, calculate the half_step_delay to use for calculating the step signal frequency.
void setupStepSignalFrequency() {
  pinMode(M1_StepSignalPin, OUTPUT);
  // pinMode(M2_StepSignalPin, OUTPUT);

  // half_step_delay is  calculated in milliseconds.
  half_step_delay = 60L * 1000L / number_of_steps_for_one_rotation / whatSpeed / 2L;

  // half_step_delay is first calculated in microseconds.
  // half_step_delay = 60L * 1000L * 1000L / number_of_steps_for_one_rotation / whatSpeed;

  // If the combination of number_of_steps_for_one_rotation and whatSpeed produces a value that is greaater than
  // what the delayMicroseconds() function can handle, half_step_delay is calculated in milliseconds.
  // if (half_step_delay > 16383) {
  //   half_step_delay = 60L * 1000L / number_of_steps_for_one_rotation / whatSpeed;
  //   useMicroseconds = false;
  // }
}

// Creates one cycle of the step signal. The period should be 2 * half_step_delay.
void takeStep() {
  digitalWrite(M1_StepSignalPin, HIGH);
  delay(half_step_delay);
  digitalWrite(M1_StepSignalPin, LOW);
  delay(half_step_delay);
}

// void loop() {
//   // Sets the High and Low periods of the PUL signal.
//   // If half_step_delay is calculated in microseconds, this sequence will be used.
//   if (useMicroseconds == true) {
//     digitalWrite(stepSignalPin, HIGH);
//     delayMicroseconds(half_step_delay);
//     digitalWrite(stepSignalPin, LOW);
//     delayMicroseconds(half_step_delay);
//   }

//   // If half_step_delay is calculated in milliseconds, this sequence will be used.
//   else {
//     digitalWrite(stepSignalPin, HIGH);
//     delay(half_step_delay);
//     digitalWrite(stepSignalPin, LOW);
//     delay(half_step_delay);
//   }
// }
