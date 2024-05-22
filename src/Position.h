#ifndef POSITION_H
#define POSITION_H

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

// Function declarations
void imusetup();
void imuloop();
void printEvent(sensors_event_t *event);
float *readSensorData();
String serializeSensorData(float yawX, float pitchY, float rollZ);

// Constants
extern double xPos;
extern double yPos;
extern double headingVel;
extern uint16_t BNO055_SAMPLERATE_DELAY_MS;
extern uint16_t PRINT_DELAY_MS;

// Variables
extern uint16_t printCount;
extern double ACCEL_VEL_TRANSITION;
extern double ACCEL_POS_TRANSITION;
extern double DEG_2_RAD;

#endif /* POSITION_H */
