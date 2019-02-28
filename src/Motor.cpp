#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int _dir, int _pwm, Encoder *_encoder)
    : dir(_dir), pwm(_pwm), encoder(_encoder), pid(PID(&pwmValue, &encoderOutput, &speed, consKp, consKi, consKd, REVERSE, P_ON_M))
{
    pinMode(dir, OUTPUT);
    pinMode(pwm, OUTPUT);
    pid.SetOutputLimits(0, 255);
    pid.SetMode(AUTOMATIC);
}

Motor::~Motor()
{
    delete encoder;
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
    }
}

void Motor::update()
{
    if (speed - encoderOutput >= maxGap)
    {
        pid.SetTunings(aggKp, aggKi, aggKd);
    }
    else
    {
        pid.SetTunings(consKp, consKi, consKd);
    }
    if (pid.Compute())
    {
        analogWrite(pwm, pwmValue);
    }
}

void Motor::brake()
{
    speed = 0;
    //analogWrite(pwm, speed);
    status = MotorStatus::Stop;
}
