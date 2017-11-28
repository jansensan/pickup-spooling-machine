#include <AccelStepper.h>
#include <Math.h>


// constants
const float MAX_STEPPER_SPEED = 1000.;
const float MAX_POT_VALUE = 1023.;


// vars
int potInPin = A0;
int potValue;

int toggleInPin = 7;
int toggleValue;

int ledOutPin = 13;

float currentSpeed = 0;
int currentDirection = 1;


// defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper;


// arduino methods
void setup() {
  // set input pins
  pinMode(potInPin, INPUT);
  pinMode(toggleInPin, INPUT_PULLUP);

  // set output pins
  pinMode(ledOutPin, OUTPUT);

  // init stepper
  stepper.setMaxSpeed(MAX_STEPPER_SPEED);

  // turn light on
  digitalWrite(ledOutPin, HIGH);
}

void loop() {
  updateMotorDirection();
  updateMotorSpeed();
  stepper.runSpeed();
}


// methods definitions
void updateMotorDirection() {
  // read toggle switch state
  toggleValue = digitalRead(toggleInPin);

  if (toggleValue == HIGH) {
    currentDirection = -1;
  } else if (toggleValue == LOW) {
    currentDirection = 1;
  }
}

void updateMotorSpeed() {
  potValue = analogRead(potInPin);

  // calculate new speed from pot input
  int newSpeed = floor((MAX_STEPPER_SPEED / MAX_POT_VALUE) * potValue);

  // limit values
  if (newSpeed > MAX_STEPPER_SPEED) {
    newSpeed = MAX_STEPPER_SPEED;
  } else if (newSpeed < 0) {
    newSpeed = 0;
  }

  // assign new speed if different
  if (newSpeed != currentSpeed) {
    currentSpeed = newSpeed;
  }

  stepper.setSpeed(currentSpeed * currentDirection);
}
