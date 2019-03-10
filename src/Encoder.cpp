#include <Encoder.h>
#include <Arduino.h>

Encoder::Encoder(int _pinA, int _pinB) : pinA(_pinA), pinB(_pinB)
{
}

//Gets encoder output for chanel A
const long Encoder::getChanelA()
{
    return pulseIn(pinA, HIGH, 100);
}

//Gets encoder output for chanel B
//Currently I don't think we need chanel B b/c it's just for getting the direction (I think)
const long Encoder::getChanelB()
{
    return pulseIn(pinB, HIGH, 100);
}