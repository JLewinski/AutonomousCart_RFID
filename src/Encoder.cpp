#include <Encoder.h>
#include <Arduino.h>

Encoder::Encoder(int _pinA, int _pinB) : pinA(_pinA), pinB(_pinB)
{
}

const long Encoder::getChanelA()
{
    return pulseIn(pinA, HIGH, 100);
}

const long Encoder::getChanelB()
{
    return pulseIn(pinB, HIGH);
}