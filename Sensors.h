#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Sensor variables
extern float MPU_6050_AccX;
extern float MPU_6050_AccY;
extern float MPU_6050_AccZ;
extern float MPU_6050_Temp;
extern float MPU_6050_GyroX;
extern float MPU_6050_GyroY;
extern float MPU_6050_GyroZ;

// BMP280 object
extern Adafruit_BMP280 bme;

// Functions
void initSensors();
void updateSensors();
void printSensors();

#endif