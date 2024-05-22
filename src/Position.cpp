#include <Adafruit_BNO055.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <ArduinoJson.h>

double xPos = 0, yPos = 0, headingVel = 0;
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; // how often to read data from the board
uint16_t PRINT_DELAY_MS = 50;             // how often to print the data
uint16_t printCount = 0;                  // counter to avoid printing every 10MS sample

// velocity = accel*dt (dt in seconds)
// position = 0.5*accel*dt^2
double ACCEL_VEL_TRANSITION = (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0;
double ACCEL_POS_TRANSITION = 0.5 * ACCEL_VEL_TRANSITION * ACCEL_VEL_TRANSITION;
double DEG_2_RAD = 0.01745329251; // trig functions require radians, BNO055 outputs degrees

sensors_event_t orientationData, linearAccelData;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void imusetup(void)
{
  Serial.println("IMU Setup Loop Reached");

  Wire.begin(26, 27);
  delay(10);
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1)
      ;
  }

  delay(1000);
}

void imuloop(void)
{
  //
  unsigned long tStart = micros();
  sensors_event_t orientationData, linearAccelData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  //  bno.getEvent(&angVelData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

  xPos = xPos + ACCEL_POS_TRANSITION * linearAccelData.acceleration.x;
  yPos = yPos + ACCEL_POS_TRANSITION * linearAccelData.acceleration.y;

  // velocity of sensor in the direction it's facing
  headingVel = ACCEL_VEL_TRANSITION * linearAccelData.acceleration.x / cos(DEG_2_RAD * orientationData.orientation.x);

  Serial.println("in imuloop right before printing");
  Serial.print("printcount val: ");
  Serial.println(printCount);
  Serial.print("print_delay_ms");
  Serial.println(PRINT_DELAY_MS);

  if (printCount * BNO055_SAMPLERATE_DELAY_MS >= PRINT_DELAY_MS)
  {
    // enough iterations have passed that we can print the latest data
    Serial.println("Orientation: ");
    Serial.print("  Yaw (z): ");
    Serial.println(orientationData.orientation.z); // Yaw
    Serial.print("  Pitch (y): ");
    Serial.println(orientationData.orientation.y); // Pitch
    Serial.print("  Roll (x): ");
    Serial.println(orientationData.orientation.x); // Roll

    printCount = 0;
  }
  else
  {

    printCount = printCount + 1;
  }
  while ((micros() - tStart) < (BNO055_SAMPLERATE_DELAY_MS * 1000))
  {
    // poll until the next sample is ready
  }
}

void printEvent(sensors_event_t *event)
{
  Serial.println();
  Serial.print(event->type);
  double x = -1000000, y = -1000000, z = -1000000; // dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER)
  {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION)
  {
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD)
  {
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if ((event->type == SENSOR_TYPE_GYROSCOPE) || (event->type == SENSOR_TYPE_ROTATION_VECTOR))
  {
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }

  Serial.print(": x= ");
  Serial.print(x);
  Serial.print(" | y= ");
  Serial.print(y);
  Serial.print(" | z= ");
  Serial.println(z);
}

float *readSensorData()
{
  sensors_event_t orientationData, linearAccelData;
  static float sensorData[3];

  sensorData[0] = orientationData.orientation.x;
  sensorData[1] = orientationData.orientation.y;
  sensorData[2] = orientationData.orientation.z;

  return sensorData;
}

String serializeSensorData(float yawX, float pitchY, float rollZ)
{
  // new sending imu data as json to ws ///////////////////////

  StaticJsonDocument<200> imudata;
  imudata["x_data"] = yawX;
  imudata["y_data"] = pitchY;
  imudata["z_data"] = rollZ;

  // Serialize json obj to str
  String jsonString;
  serializeJson(imudata, jsonString);

  // send json data over websocket
  return jsonString;

  ///////////////////////////////////////////////////////////////
}