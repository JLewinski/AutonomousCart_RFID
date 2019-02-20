#pragma once

#include <Motor.h>
#include <ProximitySensorArray.h>

typedef enum Direction
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
  MotorControl(Motor _right, Motor _left, ProximitySensorArray _sensors)
      : left(_left), right(_right), sensors(_sensors) {}
  MotorControl(const MotorControl &other) : MotorControl(other.right, other.left, other.sensors) {}
  ~MotorControl();
  //Need to create functions to controll direction and speed of motors based on the sensor data

private:
  const Motor left;
  const Motor right;
  const ProximitySensorArray sensors;
  Direction direction;
};
