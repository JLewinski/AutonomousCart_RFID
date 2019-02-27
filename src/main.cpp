#include <Arduino.h>
#include <MotorControl.h>


MotorControl *control;

void setup()
{
  Motor *right = new Motor(22, 2);
  Motor *left = new Motor(23, 3);
  ProximitySensorArray *sensors = new ProximitySensorArray(28, 30, 31, 32, 33, 29);
  control = new MotorControl(right, left, sensors);
  control->SetSpeed(130);
}

void loop()
{
  // put your main code here, to run repeatedly:
}