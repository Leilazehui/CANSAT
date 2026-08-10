#include <Arduino.h>
#include <Servo.h>

#include "Parachute.h"

Servo parachuteServo;

// -------------------------
// SERVO PIN
// -------------------------

#define SERVO_PIN 6

// Your experimentally determined
// release angle
#define RELEASE_ANGLE 70

// Initial locked position
#define INITIAL_ANGLE 0


void initParachute()
{
    parachuteServo.attach(SERVO_PIN);

    parachuteServo.write(INITIAL_ANGLE);

    delay(500);
}


void releaseParachute()
{
    Serial.println("Releasing parachute...");

    parachuteServo.write(RELEASE_ANGLE);

    delay(1000);

    Serial.println("Parachute release command completed.");
}