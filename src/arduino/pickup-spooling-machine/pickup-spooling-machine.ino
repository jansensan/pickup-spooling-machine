#include <AccelStepper.h>
#include <Math.h>


// constants
const float MAX_STEPPER_SPEED = 1000.;
const float MAX_POT_VALUE = 1023.;


// vars
int speedInputPin = A0;
int speedPotValue;

int directionInputPin = 7;
int directionToggleValue;

int motorInputPin = 8;
int motorToggleValue;
bool isMotorOn = false;

int ledOutputPin = 13;

float currentSpeed = 0;
int currentDirection = 1;


// defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper;


// arduino methods
void setup() {
  // set input pins
  pinMode(speedInputPin, INPUT);
  pinMode(directionInputPin, INPUT_PULLUP);
  pinMode(motorInputPin, INPUT_PULLUP);

  // set output pins
  pinMode(ledOutputPin, OUTPUT);

  // init stepper
  stepper.setMaxSpeed(MAX_STEPPER_SPEED);
  stepper.setSpeed(0);

  // turn light on
  digitalWrite(ledOutputPin, HIGH);
}

void loop() {
  updateMotorDirection();
  updateMotorSpeed();
  stepper.runSpeed();

  if (isMotorOn) {
    stepper.runSpeed();
  }
}


// methods definitions
void updateMotorToggle() {
  // read toggle switch state
  motorToggleValue = digitalRead(motorInputPin);

  if (motorToggleValue == HIGH) {
    isMotorOn = false;
  } else if (motorToggleValue == LOW) {
    isMotorOn = true;
  }
}

void updateMotorDirection() {
  // dont do anything if the motor is already on
  if (isMotorOn) {
    return;
  }
  
  // read toggle switch state
  directionToggleValue = digitalRead(directionInputPin);

  // assign value according to toggle position
  if (directionToggleValue == HIGH) {
    currentDirection = -1;
  } else if (directionToggleValue == LOW) {
    currentDirection = 1;
  }
}

void updateMotorSpeed() {
  speedPotValue = analogRead(speedInputPin);

  // calculate new speed from pot input
  int newSpeed = floor((MAX_STEPPER_SPEED / MAX_POT_VALUE) * speedPotValue);

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
