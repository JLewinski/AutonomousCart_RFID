#pragma once

#include <Encoder.h>

enum MotorStatus
{
  Clockwise = 0,
  CounterClockwise = 1,
  Stop = 2
};

class Motor
{
public:
  Motor(int _dir, int _pwm, Encoder _encoder);
  ~Motor();
  void clockwise();
  void counterClockwise();
  void setSpeed(double _speed);
  void brake();
  void update();

private:
  const int dir, pwm;
  int extreme = 0;
  Encoder encoder;
  double *speed = new double(0);
  double *encoderOutput = new double(0);
  double *pwmValue = new double(0);
  MotorStatus status = Stop;
  const double consKp = 1, consKi = 0.05, consKd = 0.25, aggKp = 4, aggKi = .2, aggKd = 1, maxGap = 10;
};
