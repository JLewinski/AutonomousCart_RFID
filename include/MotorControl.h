#pragma once

#include <Motor.h>
#include <ProximitySensorArray.h>
#include <Direction.h>

/**
   * Controls the motors based on sensor data
   */
class MotorControl
{
public:
  //Initializes the controller with two motors and an array of proximity sensors
  MotorControl(Motor _right, Motor _left, ProximitySensorArray _sensors);

  //Need to create functions to controll direction and speed of
  //motors based on the sensor data
  void SetSpeed(int spd);
  //Updates the offsets sent to the motors to turn or correct angle of approach
  void Update();
  //If it has a direction to turn to
  bool hasNextTurn();
  //Sets the current direction and the direction it needs to go at the next intersection
  void setTurn(Direction nextTurn, Direction currentDirection);
  //Initiates the tuning algorithm. This is currently only called from the update
  //function but has potential to be called in the system head.
  void initiateTurn();
  //Sets the motors to speed 0 in the next update loop by setting the status to paused
  void pause() { paused = true; }
  //Removes the paused status to allow the control to resume
  void resume() { paused = false; }
  //Gets the current direction of the cart based on what the motor controller knows
  Direction checkStatus();

private:
  //Motors and prox sensors
  Motor left;
  Motor right;
  ProximitySensorArray sensors;

  //private functions for updating based on input
  void updateOffset(int rf, int rb);
  int getDistance(UltrasonicSensor right, UltrasonicSensor left, bool update = true);
  int getAverageHallWidth(int currentWidth);

  //direction and speed (Updated in main)
  Direction direction = Stopped, turn = Stopped;
  int speed;

  int rightOffset, leftOffset = -1;
  bool intersection = (true), blocked = (false), paused = false;

  //get sensor data when count > countMax -> reset count to 0
  int count = 0;
  const int countMax = 10;

  const int minDif = 30, maxDif = 1, desiredDistance = 2500, offsetMax = 30, turnOffset = 50;
  const int maxCompareHistory = 500, doorCompareHistory = 100;
  int previousDistance = -1, previousDistanceDiff = -1, previousAngleDiff = -1, intersectionFlag = 0;
  const int dangerDistance = 1250, warningDistance = 600, safeDistance = 250, dangerAngle = 50, dangerFront = 3000, safeAngle = 10, maxSpeed = 110, defaultHallWidth = 9400, maxIntersection = 5, intersectionFlagInit = 2;
  int hallWidthHistory[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int hallIndex = 0;
  const int hallHistoryLength = 20;
};