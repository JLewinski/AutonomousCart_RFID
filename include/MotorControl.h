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
  MotorControl(Motor _right, Motor _left, ProximitySensorArray _sensors)
      : left(_left), right(_right), sensors(_sensors) {}
  //Need to create functions to controll direction and speed of motors based on the sensor data

  void AdjustSpeedAndDirection();
  void SetSpeed(int spd);
  void Update();

private:
  Motor left;
  Motor right;
  ProximitySensorArray sensors;
  Direction direction;
  void QuiteDif(int *tempDif);
  int speed, rightOffset, leftOffset;
  bool toggle = (false);
  int count = 0;
  const int minDif = 30, maxDif = 1, desiredDistance = 2500, countMax = 10, offsetMax = 8;
  int previousDistanceDiff = 0, previousAngleDiff = 0;
  const int dangerDistance = 1000, safeDistance = 100, dangerAngle = 50, safeAngle = 10;
};
