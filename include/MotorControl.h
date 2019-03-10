#pragma once

#include <Motor.h>
#include <ProximitySensorArray.h>

//This will need to change to a more robust direction
enum Direction
{
  Forward = 1,
  Backward = 0,
  Right = 2,
  Left = 3,
  Stopped = 4
};

class MotorControl
{
public:
  MotorControl(Motor *_right, Motor *_left, ProximitySensorArray *_sensors)
      : left(_left), right(_right), sensors(_sensors) {}
  // MotorControl(const MotorControl &other) : MotorControl(new Motor(other.right), new Motor(other.left), new ProximitySensorArray(other.sensors) {}
  ~MotorControl();
  //Need to create functions to controll direction and speed of motors based on the sensor data

  void AdjustSpeedAndDirection();
  void SetSpeed(int spd);
  void Update();

private:
  Motor *left;
  Motor *right;
  const ProximitySensorArray *sensors;
  Direction direction;
  int speed, rightOffset, leftOffset;
  int angle;
};
