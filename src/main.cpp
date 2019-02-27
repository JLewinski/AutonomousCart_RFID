#include <Arduino.h>
#include <MotorControl.h>

MotorControl *control;

void setup()
{
  Motor *right = new Motor(8, 9, 2);
  Motor *left = new Motor(10, 11, 3);
  ProximitySensorArray *sensors = new ProximitySensorArray(7, 12, 13, 14, 15, 16);
  control = new MotorControl(right, left, sensors);
}

void loop()
{
  // put your main code here, to run repeatedly:
}