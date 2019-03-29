#define DEBUG
#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int _dir, int _pwm, Encoder _encoder)
    : dir(_dir), pwm(_pwm), encoder(_encoder)
{
    // pwmValue = 0;
    pinMode(dir, OUTPUT);
    pinMode(pwm, OUTPUT);
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
void Motor::setSpeed(double _speed)
{
    if (_speed < 0)
    {
        *speed = 0;
    }
    else if (_speed > 55)
    {
        *speed = 55.0;
    }
    else
    {
        *speed = _speed;
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
    Serial.println(*speed);
#endif
}

//Gets the encoder output and updating the PWM value
void Motor::update()
{
    long enOut = encoder.getAvg();
    Serial.print("Encoder: ");
    Serial.print(enOut);
    Serial.print(" Previous PWM: ");
    Serial.print(*pwmValue);
    if (enOut > 100)
    {
        *pwmValue = 80;
    }
    else
    {
        double error = (enOut - *speed) / *speed;
        Serial.print(" Error: ");
        Serial.print(error);
        double inc = *pwmValue * error / *speed;
        if (inc > 50)
        {
            inc = 25;
        }
        *pwmValue += inc;

        if (*pwmValue >= 255)
        {
            //Should probably stop
            *pwmValue = 255;
        }
        else if (*pwmValue <= 1 && *speed > 0)
        {
            //just because
            *pwmValue = 10;
        }
    }
    Serial.print(" New PWM: ");
    Serial.println(*pwmValue);
    analogWrite(pwm, *pwmValue);
}

//Emergency Brake
void Motor::brake()
{
    speed = 0;
    pwmValue = 0;
    analogWrite(pwm, 0);
    status = MotorStatus::Stop;
}
