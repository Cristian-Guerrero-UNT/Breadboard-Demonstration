/*
This sketch is an example of how interrupts can be used to gather
user input via external interrupts and pin change interrupts.
It also allows for testing the logic needed for implementing
the limit switches and deciding 
*/

#include <PinChangeInterrupt.h>

// Pin definitions
const byte EMERGENCY_LED = 11;
const byte ENABLE_LED = 10;
const byte CW_LED = 9;
const byte CCW_LED = 8;

const byte EMERGENCY_BUTTON = 2;
const byte ENABLE_BUTTON = 7;
const byte CW_BUTTON = 6;
const byte CCW_BUTTON = 5;

const byte TOP_LIMIT_SWITCH = 4;
const byte BOTTOM_LIMIT_SWITCH = 3;

// Define volatile variables
volatile byte state_of_EMERGENCY_BUTTON = 0;
volatile byte state_of_ENABLE_BUTTON = 0;
volatile byte state_of_CW_BUTTON = 0;
volatile byte state_of_CCW_BUTTON = 0;
volatile byte state_of_TOP_LIMIT_SWITCH = 0;
volatile byte state_of_BOTTOM_LIMIT_SWITCH = 0;

byte seconds_for_LED_activation = 15;

// Define the actions for each button
void actionForButton(byte buttonPin)
{
  switch (buttonPin)
  {
  case EMERGENCY_BUTTON:
    // Stop stepper motor
    // Red LED On
    Serial.println("Red LED On");
    digitalWrite(EMERGENCY_LED, HIGH);
    delay(1000);
    digitalWrite(EMERGENCY_LED, LOW);
    state_of_EMERGENCY_BUTTON = 0;
    break;
  case ENABLE_BUTTON:
    // Enable stepper motor
    // Green LED On
    Serial.println("Green LED On");
    digitalWrite(ENABLE_LED, HIGH);
    delay(1000);
    digitalWrite(ENABLE_LED, LOW);
    state_of_ENABLE_BUTTON = 0;
    break;
  default:
    // If we enter here, send error message
    Serial.println("There has been an error within the switch statement.");
    break;
  }
}

// Interrupt service routines
void emergency_stop()
{
  // Red LED On
  state_of_EMERGENCY_BUTTON = EMERGENCY_BUTTON;
}

void pinChangeInterrupt1()
{
  // Green LED On
  state_of_ENABLE_BUTTON = ENABLE_BUTTON;
}

void pinChangeInterrupt2()
{
  // Check if the corresponding limit switch is NOT pressed down.
  // Set flag to enable rotation if so.
  if (state_of_TOP_LIMIT_SWITCH == 0)
  {
    state_of_CW_BUTTON = CW_BUTTON;
  }
  else
  {
    Serial.println("Carriage is on the Top Limit Switch. Select the other direction.");
  }
}

void pinChangeInterrupt3()
{
  // Check if the corresponding limit switch is NOT pressed down.
  // Set flag to enable rotation if so.
  if (state_of_BOTTOM_LIMIT_SWITCH == 0)
  {
    state_of_CCW_BUTTON = CCW_BUTTON;
  }
  else
  {
    Serial.println("Carriage is on the Bottom Limit Switch. Select the other direction.");
  }
}

void pinChangeInterrupt4()
{
  // TOP_LIMIT_SWITCH has been pressed down.
  state_of_TOP_LIMIT_SWITCH = 1;
}

void pinChangeInterrupt5()
{
  // BOTTOM_LIMIT_SWITCH has been pressed down.
  state_of_BOTTOM_LIMIT_SWITCH = 1;
}

void setupButtons()
{
  // Set pins as output for LEDs.
  pinMode(EMERGENCY_LED, OUTPUT);
  pinMode(ENABLE_LED, OUTPUT);
  pinMode(CW_LED, OUTPUT);
  pinMode(CCW_LED, OUTPUT);
  // Set button pins as input.
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(CW_BUTTON, INPUT_PULLUP);
  pinMode(CCW_BUTTON, INPUT_PULLUP);
  // Set limit switches as input.
  pinMode(TOP_LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(BOTTOM_LIMIT_SWITCH, INPUT_PULLUP);

  // Attach interrupts to pins.
  // Arduino Interrupts allow for LOW, HIGH, CHANGE, RISING, AND FALLING as valid modes.
  // The PinChangeInterrupt library has RISING, FALLING, and CHANGE as valid modes of operation.
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergency_stop, FALLING);
  attachPCINT(digitalPinToPCINT(ENABLE_BUTTON), pinChangeInterrupt1, FALLING);
  attachPCINT(digitalPinToPCINT(CW_BUTTON), pinChangeInterrupt2, FALLING);
  attachPCINT(digitalPinToPCINT(CCW_BUTTON), pinChangeInterrupt3, FALLING);
  attachPCINT(digitalPinToPCINT(TOP_LIMIT_SWITCH), pinChangeInterrupt4, FALLING);
  attachPCINT(digitalPinToPCINT(BOTTOM_LIMIT_SWITCH), pinChangeInterrupt5, FALLING);
}

// This function should execute 2 rotations of the stepper motor.
// 
void executeRotation(byte buttonPin)
{
  unsigned int counter = 0;
  byte button = buttonPin;

  // Take steps in the direction called for by the user.
  // Turn the corresponding LED on for 15 seconds.
  if (button == CW_BUTTON)
  {
    Serial.println("Yellow1 LED On");
    digitalWrite(CW_LED, HIGH);
    while (counter < seconds_for_LED_activation && state_of_TOP_LIMIT_SWITCH == 0)
    {
      delay(1000);
      counter++;
    }
    digitalWrite(CW_LED, LOW);
  }
  else
  { // CCW button was pressed.
    Serial.println("Yellow2 LED On");
    digitalWrite(CCW_LED, HIGH);
    while (counter < seconds_for_LED_activation && state_of_BOTTOM_LIMIT_SWITCH == 0)
    {
      delay(1000);
      counter++;
    }
    digitalWrite(CCW_LED, LOW);
  }

  // Clear the flag after a  successful rotation. The carriage should no longer be on the limit switch.
  if (state_of_TOP_LIMIT_SWITCH != 0)
  {
    state_of_TOP_LIMIT_SWITCH = 0;
  }
  if (state_of_BOTTOM_LIMIT_SWITCH != 0)
  {
    state_of_BOTTOM_LIMIT_SWITCH = 0;
  }
}

void setup()
{
  // Initialize all the buttons.
  setupButtons();
  // Initialize serial communication.
  Serial.begin(115200);
}

void loop()
{
  // Check to  see if any buttons were pressed.
  // If a button was pressed call the relevant function for output.
  if (state_of_EMERGENCY_BUTTON != 0)
  {
    actionForButton(state_of_EMERGENCY_BUTTON);
  }
  else if (state_of_ENABLE_BUTTON != 0)
  {
    actionForButton(state_of_ENABLE_BUTTON);
  }
  else if (state_of_CW_BUTTON != 0)
  {
    executeRotation(state_of_CW_BUTTON);
  }
  else if (state_of_CCW_BUTTON != 0)
  {
    executeRotation(state_of_CCW_BUTTON);
  }
  delay(500);
}
