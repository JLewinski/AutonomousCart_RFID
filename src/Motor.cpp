#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int _dir, int _pwm, Encoder _encoder)
    : dir(_dir), pwm(_pwm), encoder(_encoder), pid(PID(&encoderOutput, &pwmValue, &speed, consKp, consKi, consKd, DIRECT))
{
    pwmValue = 0;
    pinMode(dir, OUTPUT);
    pinMode(pwm, OUTPUT);
    // pid.SetOutputLimits(30, 255);
    pid.SetMode(AUTOMATIC);
}

Motor::~Motor()
{
}

//Start moving clockwise at the same set speed.
//It would be best to set the speed to 0 before doing this
void Motor::clockwise()
{
    digitalWrite(dir, HIGH);
    status = MotorStatus::Clockwise;
}

//Start moving counterclockwise at the same set speed.
//It would be best to set the speed to 0 before doing this
void Motor::counterClockwise()
{
    digitalWrite(dir, LOW);
    status = MotorStatus::CounterClockwise;
}

//Set the desired encoder output
void Motor::setSpeed(int _speed)
{
    if (_speed < 0)
    {
        speed = 0;
    }
    else if (_speed > 55)
    {
        speed = 55;
    }
    else
    {
        speed = _speed;
    }
#ifdef DEBUG
    if (pwm == 5)
    {
        Serial.print("Right Speed: ");
    }
    else
    {
        Serial.print("Left Speed: ");
    }
    Serial.println(speed);
#endif
}

//Gets the encoder output and updating the PWM value
void Motor::update()
{
    encoderOutput = encoder.getChanelA();
#ifdef DEBUG
    if (pwm == 5)
    {
        Serial.print("Right Encoder: ");
    }
    else
    {
        Serial.print("Left Encoder: ");
    }
    Serial.println(encoderOutput);
#endif
    // if (encoderOutput == 0)
    // {
    //     encoderOutput = 90;
    // }
    // if (speed - encoderOutput >= maxGap)
    // {
    //     pid.SetTunings(aggKp, aggKi, aggKd);
    // }
    // else
    // {
    //     pid.SetTunings(consKp, consKi, consKd);
    // }

    //The pid object that does all the calculations.
    //It will only update the value if a set amount of
    //time was past.
    if (pid.Compute())
    {

        if (speed > 9 && pwmValue == 0)
        {
            pwmValue = 80;
        }
#ifdef DEBUG
        if (pwm == 5)
        {
            Serial.print("Right PWM: ");
        }
        else
        {
            Serial.print("Left PWM: ");
        }

        Serial.println(pwmValue);
#endif
        analogWrite(pwm, pwmValue);
    }
}

//Emergency Brake
void Motor::brake()
{
    speed = 0;
    pwmValue = 0;
    analogWrite(pwm, 0);
    status = MotorStatus::Stop;
}
