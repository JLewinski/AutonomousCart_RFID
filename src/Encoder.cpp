#include <Encoder.h>
#include <Arduino.h>

Encoder::Encoder(int _pinA, int _pinB) : pinA(_pinA), pinB(_pinB)
{
}

const long Encoder::getChanelA()
{
    return pulseIn(pinA, HIGH);
}

const long Encoder::getChanelB()
{
    return pulseIn(pinB, HIGH);
}

void Encoder::update()
{
    currentValue = getChanelA();
}