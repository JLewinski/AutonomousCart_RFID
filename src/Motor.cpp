#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int _in_1, int _in_2, int _pwm) : in_1(_in_1), in_2(_in_2), pwm(_pwm)
{
    pinMode(in_1, OUTPUT);
    pinMode(in_2, OUTPUT);
    pinMode(pwm, OUTPUT);
}

Motor::~Motor()
{
}

void Motor::clockwise()
{
    digitalWrite(in_1, HIGH);
    digitalWrite(in_2, LOW);
    status = MotorStatus::Clockwise;
}

void Motor::counterClockwise()
{
    digitalWrite(in_1, LOW);
    digitalWrite(in_2, HIGH);
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
    digitalWrite(in_1, HIGH);
    digitalWrite(in_2, HIGH);
    status = MotorStatus::Stop;
}
