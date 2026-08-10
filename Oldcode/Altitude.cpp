#include "Altitude.h"
#include "Sensors.h"

float launchAltitude = 0;
float currentAltitude = 0;
float relativeAltitude = 0;

float previousAltitude = 0;

float maxAltitude = 0;

float verticalSpeed = 0;

float deltaAltitude = 0;

unsigned long previousTime = 0;


void initAltitude()
{
    launchAltitude = bme.readAltitude(1013.25);

    currentAltitude = launchAltitude;
    relativeAltitude = 0;
    maxAltitude = 0;

    previousAltitude = launchAltitude;

    previousTime = millis();
}


void updateAltitude()
{
    currentAltitude = bme.readAltitude(1013.25);

    relativeAltitude =
        currentAltitude - launchAltitude;


    if (relativeAltitude > maxAltitude)
    {
        maxAltitude = relativeAltitude;
    }


    unsigned long currentTime = millis();

    float dt =
        (currentTime - previousTime) / 1000.0;


    if (dt > 0)
    {
        deltaAltitude =
            currentAltitude - previousAltitude;

        verticalSpeed =
            deltaAltitude / dt;
    }


    previousAltitude = currentAltitude;

    previousTime = currentTime;
}


float getCurrentAltitude()
{
    return currentAltitude;
}


float getRelativeAltitude()
{
    return relativeAltitude;
}


float getMaxAltitude()
{
    return maxAltitude;
}


float getVerticalSpeed()
{
    return verticalSpeed;
}
