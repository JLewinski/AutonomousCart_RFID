#pragma once

#include <Motor.h>
#include <ProximitySensorArray.h>
#include <Direction.h>

class MotorControl
{
public:
  MotorControl(Motor _right, Motor _left, ProximitySensorArray _sensors)
      : left(_left), right(_right), sensors(_sensors) {}
  //Need to create functions to controll direction and speed of motors based on the sensor data
  void SetInitialDirection();
  void SetNextTurn(Direction dir);
  void SetSpeed(int spd);
  void Update();
  void initiateTurn();
  bool hasNextTurn();
  void setTurn(Direction d);

private:
  //Motors and prox sensors
  Motor left;
  Motor right;
  ProximitySensorArray sensors;

  //private functions for updating based on input
  void updateOffset(int rf, int rb);
  int getDistance(UltrasonicSensor right, UltrasonicSensor left);
  int getAverageHallWidth(int currentWidth);

  //direction and speed (Updated in main)
  Direction direction = Stopped, turn = Stopped;
  int speed;

  int rightOffset, leftOffset = -1;
  bool intersection = (true), blocked = (false);

  //get sensor data when count > countMax -> reset count to 0
  int count = 0;
  const int countMax = 10;


  const int minDif = 30, maxDif = 1, desiredDistance = 2500, offsetMax = 30;
  const int maxCompareHistory = 500, doorCompareHistory = 100;
  int previousDistanceDiff = 0, previousAngleDiff = 0;
  const int dangerDistance = 1000, safeDistance = 100, dangerAngle = 50, dangerFront = 3000, safeAngle = 10, maxSpeed = 110, defaultHallWidth = 9650;
  int hallWidthHistory[40] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int hallIndex = 0;
  const int hallHistoryLength = 40;
};