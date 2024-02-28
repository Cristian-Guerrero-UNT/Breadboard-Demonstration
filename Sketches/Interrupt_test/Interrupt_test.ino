/*
This sketch is an example of how interrupts can be used to gather
user input via external interrupts and pin change interrupts.
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

// Define volatile variables
volatile byte state_of_EMERGENCY_BUTTON = 0;
volatile byte state_of_ENABLE_BUTTON = 0;
volatile byte state_of_CW_BUTTON = 0;
volatile byte state_of_CCW_BUTTON = 0;

// Define the actions for each button
void actionForButton(byte buttonPin) {
  switch (buttonPin) {
    case EMERGENCY_BUTTON:
      // Drive Stepper Motor Clockwise
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
    case CW_BUTTON:
      // Drive Stepper Motor Clockwise
      // Yellow1 LED On
      Serial.println("Yellow1 LED On");
      digitalWrite(CW_LED, HIGH);
      delay(1000);
      digitalWrite(CW_LED, LOW);
      state_of_CW_BUTTON = 0;
      break;
    case CCW_BUTTON:
      // Drive Stepper Motor Clockwise
      // Yellow2 LED On
      Serial.println("Yellow2 LED On");
      digitalWrite(CCW_LED, HIGH);
      delay(1000);
      digitalWrite(CCW_LED, LOW);
      state_of_CCW_BUTTON = 0;
      break;

    default:
      // If we enter here message Error
      Serial.println("There has been an error within the switch statement.");
      break;
  }
}

void emergency_stop() {
  // Red LED On
  state_of_EMERGENCY_BUTTON = EMERGENCY_BUTTON;
}

// Interrupt service routines
void pinChangeInterrupt1() {
  // Green LED On
  state_of_ENABLE_BUTTON = ENABLE_BUTTON;
}

void pinChangeInterrupt2() {
  // Yellow1 LED On
  state_of_CW_BUTTON = CW_BUTTON;
}

void pinChangeInterrupt3() {
  // Yellow2 LED On
  state_of_CCW_BUTTON = CCW_BUTTON;
}

void setupButtons() {
  // Set button pins as output for LEDs.
  pinMode(EMERGENCY_LED, OUTPUT);
  pinMode(ENABLE_LED, OUTPUT);
  pinMode(CW_LED, OUTPUT);
  pinMode(CCW_LED, OUTPUT);
  // Set button pins as input.
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(CW_BUTTON, INPUT_PULLUP);
  pinMode(CCW_BUTTON, INPUT_PULLUP);

  // Attach interrupts to pins.
  // Arduino Interrupts allow for LOW, HIGH, CHANGE, RISING, AND FALLING as valid modes.
  // The PinChangeInterrupt library has RISING, FALLING, and CHANGE as valid modes of operation.
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergency_stop, FALLING);
  attachPCINT(digitalPinToPCINT(ENABLE_BUTTON), pinChangeInterrupt1, FALLING);
  attachPCINT(digitalPinToPCINT(CW_BUTTON), pinChangeInterrupt2, FALLING);
  attachPCINT(digitalPinToPCINT(CCW_BUTTON), pinChangeInterrupt3, FALLING);
}

void setup() {
  // Initialize all the buttons.
  setupButtons();
  // Initialize serial communication.
  Serial.begin(115200);
}

void loop() {
  if (state_of_EMERGENCY_BUTTON != 0) {
    actionForButton(state_of_EMERGENCY_BUTTON);
  }
  else if (state_of_ENABLE_BUTTON != 0) {
    actionForButton(state_of_ENABLE_BUTTON);
  }
  else if (state_of_CW_BUTTON != 0) {
    actionForButton(state_of_CW_BUTTON);
  }
  else if (state_of_CCW_BUTTON != 0) {
    actionForButton(state_of_CCW_BUTTON);
  }
  delay(500);
}

