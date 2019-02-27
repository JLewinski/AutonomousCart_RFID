#include <ProximitySensorArray.h>
#include <Arduino.h>

ProximitySensorArray::ProximitySensorArray(int _echo, int lb, int lf, int rb, int rf, int f)
    : echo(_echo), leftBack(lb), leftFront(lf), rightBack(rb), rightFront(rf), front(f)
{
    pinMode(leftBack, OUTPUT);
    pinMode(leftFront, OUTPUT);
    pinMode(rightBack, OUTPUT);
    pinMode(rightFront, OUTPUT);
    pinMode(front, OUTPUT);
    pinMode(_echo, INPUT);
}

ProximitySensorArray::~ProximitySensorArray()
{
    delete[] leftBackValues;
    delete[] leftFrontValues;
    delete[] rightBackValues;
    delete[] rightFrontValues;
    delete[] frontValues;
}

int *ProximitySensorArray::read()
{
    shift(leftBackValues, leftBack);
    shift(leftFrontValues, leftFront);
    shift(rightBackValues, rightBack);
    shift(rightFrontValues, rightFront);
    shift(frontValues, front);
}

const long ProximitySensorArray::read(int pin)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
    return pulseIn(echo, HIGH);
}

void ProximitySensorArray::shift(long *values, int pin)
{
    for (int i = 4; i >= 1; i--)
    {
        values[i] = values[i - 1];
    }
    values[0] = read(pin);
}