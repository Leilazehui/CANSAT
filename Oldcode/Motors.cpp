#include "Motors.h"

// -------------------------
// L298N MOTOR PINS
// -------------------------

// Left motor
#define ENA_PIN 10
#define IN1_PIN 9
#define IN2_PIN 8

// Right motor
#define ENB_PIN 3
#define IN3_PIN 4
#define IN4_PIN 5


// -------------------------
// INITIALISE
// -------------------------

void initMotors()
{
    pinMode(ENA_PIN, OUTPUT);
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);

    pinMode(ENB_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);

    stopMotors();
}


// -------------------------
// LEFT MOTOR
// -------------------------

void setLeftMotor(byte speed, bool forward)
{
    analogWrite(ENA_PIN, speed);

    if (forward)
    {
        digitalWrite(IN1_PIN, HIGH);
        digitalWrite(IN2_PIN, LOW);
    }
    else
    {
        digitalWrite(IN1_PIN, LOW);
        digitalWrite(IN2_PIN, HIGH);
    }
}


// -------------------------
// RIGHT MOTOR
// -------------------------

void setRightMotor(byte speed, bool forward)
{
    analogWrite(ENB_PIN, speed);

    if (forward)
    {
        digitalWrite(IN3_PIN, HIGH);
        digitalWrite(IN4_PIN, LOW);
    }
    else
    {
        digitalWrite(IN3_PIN, LOW);
        digitalWrite(IN4_PIN, HIGH);
    }
}


// -------------------------
// MOVEMENT FUNCTIONS
// -------------------------

void moveForward(byte speed)
{
    setLeftMotor(speed, true);
    setRightMotor(speed, true);
}


void moveBackward(byte speed)
{
    setLeftMotor(speed, false);
    setRightMotor(speed, false);
}


void turnLeft(byte speed)
{
    // Left wheel slower
    // Right wheel faster

    setLeftMotor(speed * 0.3, true);
    setRightMotor(speed, true);
}


void turnRight(byte speed)
{
    // Left wheel faster
    // Right wheel slower

    setLeftMotor(speed, true);
    setRightMotor(speed * 0.3, true);
}


void stopMotors()
{
    analogWrite(ENA_PIN, 0);
    analogWrite(ENB_PIN, 0);

    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);

    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);
}
