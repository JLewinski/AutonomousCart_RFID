#include <Arduino.h>
#include <MotorControl.h>

MotorControl *control;

void setup()
{
  Motor *right = new Motor(22, 5, new Encoder(24, 26));
  Motor *left = new Motor(23, 6, new Encoder(25, 27));
  ProximitySensorArray *sensors = new ProximitySensorArray(28, 29, 30, 31, 32, 33, 34, 35, 36, 37);
  control = new MotorControl(right, left, sensors);
  control->SetSpeed(130);
}

void loop()
{
  // put your main code here, to run repeatedly:
  control->Update();
}