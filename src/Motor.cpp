#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int _dir, int _pwm) : dir(_dir), pwm(_pwm)
{
    pinMode(dir, OUTPUT);
    pinMode(pwm, OUTPUT);
}

Motor::~Motor()
{
}

void Motor::clockwise()
{
    digitalWrite(dir, HIGH);
    status = MotorStatus::Clockwise;
}

void Motor::counterClockwise()
{
    digitalWrite(dir, LOW);
    status = MotorStatus::CounterClockwise;
}

void Motor::setSpeed(int _speed)
{
    if (_speed > 0 && _speed <= 255)
    {
        speed = _speed;
        analogWrite(pwm, speed);
    }
}

void Motor::brake()
{
    speed = 0;
    analogWrite(pwm, speed);
    status = MotorStatus::Stop;
}
