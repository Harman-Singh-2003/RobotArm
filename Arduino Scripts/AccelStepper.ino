#include <AccelStepper.h>

int stepPin = 3;
int dirPin = 2;

AccelStepper motor1(1, stepPin, dirPin);

void setup() {
  // put your setup code here, to run once:
  motor1.setMaxSpeed(800);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  motor1.setSpeed(-50);
  motor1.runSpeed();
}