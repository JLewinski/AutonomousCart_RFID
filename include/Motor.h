#pragma once

#include <Encoder.h>
#include <PID_v1.h>

enum MotorStatus
{
  Clockwise = 0,
  CounterClockwise = 1,
  Stop = 2
};

class Motor
{
public:
  Motor(int _dir, int _pwm, Encoder *_encoder);
  ~Motor();
  void clockwise();
  void counterClockwise();
  void setSpeed(int _speed);
  void brake();
  void update();

private:
  const int dir, pwm;
  Encoder *encoder;
  double speed = 0;
  double encoderOutput;
  double pwmValue;
  MotorStatus status = Stop;
  PID pid;
  const double consKp = 1, consKi = 0.05, consKd = 0.25, aggKp = 4, aggKi = .2, aggKd = 1, maxGap = 10;
};
