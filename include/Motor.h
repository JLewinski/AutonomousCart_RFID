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
  void setSpeed(int _speed);
  void brake();
  void update();

private:
  const int dir, pwm, maxSpeed = 110;
  Encoder encoder;
  int desieredEncoderOutput = 0;
  int encoderOutput = 0;
  int pwmValue = 0;
  MotorStatus status = Stop;
};
