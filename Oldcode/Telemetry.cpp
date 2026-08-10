#include "Telemetry.h"
#include "Sensors.h"

#include "Altitude.h"
#include <SoftwareSerial.h>

SoftwareSerial XBee(A2,A3);


void initTelemetry() {
  // put your setup code here, to run once:
  XBee.begin(9600);

}

void sendTelemetry() {
  // put your main code here, to run repeatedly:
  XBee.print("MPU-6050 Acc X-Axis   ");
  XBee.print(MPU_6050_AccX, 2);
  XBee.println(" [g]");

  XBee.print("MPU-6050 Acc Y-Axis   ");
  XBee.print(MPU_6050_AccY,2);
  XBee.println(" [g]");

  XBee.print("MPU-6050 Acc Z-Axis   ");
  XBee.print(MPU_6050_AccZ, 2);
  XBee.println(" [g]");

  XBee.print("MPU_6050 GyroX    ");
  XBee.print(MPU_6050_GyroX, 2);
  XBee.println(" [deg/s]");

  XBee.print("MPU_6050 GyroY    ");
  XBee.print(MPU_6050_GyroY, 2);
  XBee.println(" [deg/s]");

  XBee.print("MPU_6050 Gyro Z-Axis   ");
  XBee.print(MPU_6050_GyroZ, 2);
  XBee.println(" [deg/s]");

  XBee.print("MPU-6050 Temperature    ");
  XBee.print(MPU_6050_Temp,1);
  XBee.print(" [C]");

  XBee.println("----------");

  XBee.print("BMP280 Temperature  = ");
  XBee.print(bme.readTemperature());
  XBee.println(" [C]");

  XBee.print("BMP280 Pressure  = ");
  XBee.print(bme.readPressure()/100);
  XBee.println(" [hPa]");

  XBee.print("BMP280 Approc altitude  = ");
  XBee.print(bme.readAltitude(1013.25));
  XBee.println("  [m]");

  XBee.println("----------");

  XBee.print("Current Height  = ");
  XBee.print(getRelativeAltitude());
  XBee.println("  [m]");

  XBee.print("Maximum Height  = ");
  XBee.print(getMaxAltitude());
  XBee.println("  [m]");

  XBee.print("Vertical Speed  = ");
  XBee.print(getVerticalSpeed());
  XBee.println(" [m/s]");



}
