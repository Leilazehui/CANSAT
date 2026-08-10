#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

void initMotors();

void stopMotors();
void moveForward(byte speed);
void moveBackward(byte speed);
void turnLeft(byte speed);
void turnRight(byte speed);

void setLeftMotor(byte speed, bool forward);
void setRightMotor(byte speed, bool forward);

#endif
