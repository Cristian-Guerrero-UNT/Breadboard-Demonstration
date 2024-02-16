#include <TimerOne.h>

#define EMERG_BUTTON 2
#define STEPpin 6
#define Enable_button 0
#define CW_button 1
#define CCW_button 8

uint8_t button_state = 1;
unsigned int PWMfreq = 200;
unsigned int step_delay = 1000000/(PWMfreq); //result in microseconds
long int buttonReadTime = 0;
const int buttonReadDelay = 25; // delay for 25 ms
bool motorState = false; //variable for button press to enable/disable motor

void setup() {
  // 
  pinMode(CW_button, INPUT_PULLUP);
  pinMode(CCW_button, INPUT_PULLUP);
  pinMode(EMERG_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EMERG_BUTTON), emerg_stop, LOW);
  pinMode(Enable_button, INPUT_PULLUP);
  pinMode(10,OUTPUT);
  //Timer1.initialize(step_delay);  // 5000us = 200hz

}

void loop() {
  //
    if(!digitalRead(Enable_button)) {
    changeState();
  }

  //Serial.println(digitalRead(BUTTON));
  //Serial.println("Step delay: " + String(step_delay) +" uS");
  if(millis() > buttonReadTime + buttonReadDelay) // poll the push button every 25ms to see if it is still pressed
  {
    if(!digitalRead(CW_button) != !digitalRead(CCW_button))
    {
      //Timer1.pwm(10,512);
    }
    else
    {
      //Timer1.pwm(10,0);
    }
    buttonReadTime = millis();
  }

}

//##########################################################################
void changeState() 
//##########################################################################
{
  motorState = !motorState;
  setMotor(motorState);
}

//##########################################################################
void selectFrequency(int value)
//##########################################################################
{  
  PWMfreq = value;
  step_delay = 1000000/PWMfreq;
  //Timer1.initialize(step_delay);
}

//##########################################################################
void emerg_stop()
//##########################################################################
{
  Axis[currentAxis].disable() ;
}

