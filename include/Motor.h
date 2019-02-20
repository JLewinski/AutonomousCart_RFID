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
  Motor(int _in_1, int _in_2, int _pwm);
  ~Motor();
  void clockwise();
  void counterClockwise();
  void setSpeed(int _speed);
  void brake();

private:
  const int in_1;
  const int in_2;
  const int pwm;
  int speed = 0;
  MotorStatus status = Stop;
};
