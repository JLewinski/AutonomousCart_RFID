#pragma once
enum MotorStatus
{
  Clockwise = 0,
  CounterClockwise = 1,
  Stop = 2
};

class Motor
{
public:
  Motor(int _dir, int _pwm);
  ~Motor();
  void clockwise();
  void counterClockwise();
  void setSpeed(int _speed);
  void brake();

private:
  const int dir;
  const int pwm;
  int speed = 0;
  MotorStatus status = Stop;
};
