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
    Motor(int _in_1, int _in_2, int _pwm) : in_1(_in_1), in_2(_in_2), pwm(_pwm), speed(0) {}
    Motor(int _in_1, int _in_2, int _pwm, int _speed) : in_1(_in_1), in_2(_in_2), pwm(_pwm), speed(_speed) {}
    ~Motor();
    void clockwise();
    void counterClockwise();
    void setSpeed(int _speed);
    void brake();

  private:
    const int in_1;
    const int in_2;
    const int pwm;
    int speed;
    MotorStatus status;
};
