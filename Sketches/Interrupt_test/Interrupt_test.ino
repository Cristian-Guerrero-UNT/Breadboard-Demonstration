#include <PinChangeInterrupt.h>

// Pin definitions
#define EMERGENCY_LED 11
#define ENABLE_LED 10
#define CW_LED 9
#define CCW_LED 8

#define EMERGENCY_BUTTON 2
#define ENABLE_BUTTON 7
#define CW_BUTTON 6
#define CCW_BUTTON 5

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
      break;
    case ENABLE_BUTTON:
      // Enable stepper motor
      // Green LED On
      Serial.println("Green LED On");
      digitalWrite(ENABLE_LED, HIGH);
      delay(1000);
      digitalWrite(ENABLE_LED, LOW);
      break;
    case CW_BUTTON:
      // Drive Stepper Motor Clockwise
      // Yellow1 LED On
      Serial.println("Yellow2 LED On");
      digitalWrite(CW_LED, HIGH);
      delay(1000);
      digitalWrite(CW_LED, LOW);
      break;
    case CCW_BUTTON:
      // Drive Stepper Motor Clockwise
      // Yellow2 LED On
      Serial.println("Yellow2 LED On");
      digitalWrite(CCW_LED, HIGH);
      delay(1000);
      digitalWrite(CCW_LED, LOW);
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
  actionForButton(state_of_EMERGENCY_BUTTON);
}

// Interrupt service routines
void pinChangeInterrupt1() {
  // Green LED On
  state_of_ENABLE_BUTTON = ENABLE_BUTTON;
  actionForButton(state_of_ENABLE_BUTTON);
}

void pinChangeInterrupt2() {
  // Yellow1 LED On
  state_of_CW_BUTTON = CW_BUTTON;
  actionForButton(state_of_CW_BUTTON);
}

void pinChangeInterrupt3() {
  // Yellow2 LED On
  state_of_CCW_BUTTON = CCW_BUTTON;
  actionForButton(state_of_CCW_BUTTON);
}

void setupButtons() {
  // Set button pins as input
  pinMode(EMERGENCY_LED, OUTPUT);
  pinMode(ENABLE_LED, OUTPUT);
  pinMode(CW_LED, OUTPUT);
  pinMode(CCW_LED, OUTPUT);
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(CW_BUTTON, INPUT_PULLUP);
  pinMode(CCW_BUTTON, INPUT_PULLUP);

  // Attach pin change interrupt
  // The PinChangeInterrupt library has RISING, FALLING, and CHANGE as valid modes of operation.
  // Arduino Interrupts allow for LOW, HIGH, CHANGE, RISING, AND FALLING as valid modes.
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergency_stop, LOW);
  attachPCINT(digitalPinToPCINT(ENABLE_BUTTON), pinChangeInterrupt1, FALLING);
  attachPCINT(digitalPinToPCINT(CW_BUTTON), pinChangeInterrupt2, FALLING);
  attachPCINT(digitalPinToPCINT(CCW_BUTTON), pinChangeInterrupt3, FALLING);
}

void setup() {
  setupButtons();

  // Initialize serial communication
  Serial.begin(115200);
}

void loop() {
  // Empty
  // delay(500);
}


/*
// Define the pins for the buttons
const int buttonPins[] = {2, 3, 4, 5, 6, 7};
const int numButtons = 6;

// Define the actions for each button
void actionForButton(int buttonPin) {
  switch (buttonPin) {
    case 2:
      // Action for button connected to pin 2
      break;
    case 3:
      // Action for button connected to pin 3
      break;
    case 4:
      // Action for button connected to pin 4
      break;
    case 5:
      // Action for button connected to pin 5
      break;
    case 6:
      // Action for button connected to pin 6
      break;
    case 7:
      // Action for button connected to pin 7
      break;
    default:
      // Default action
      break;
  }
}

// Interrupt service routine
void pinChangeInterrupt() {
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      actionForButton(buttonPins[i]);
    }
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set button pins as input
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Attach pin change interrupt
  attachInterrupt(digitalPinToInterrupt(buttonPins[0]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[1]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[2]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[3]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[4]), pinChangeInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPins[5]), pinChangeInterrupt, CHANGE);
}

void loop() {
  // Main program loop
  // You can add other tasks here if needed
}

*/