#include <Encoder.h>
#include <Arduino.h>

Encoder::Encoder(int _pinA, int _pinB) : pinA(_pinA), pinB(_pinB)
{
}

const long Encoder::getAvg()
{
    long cha = getChanel(pinA);
    long chb = getChanel(pinB);
    if (cha == 0)
    {
        cha = chb;
#ifdef DEBUG
        Serial.println("Chanel A = 0");
#endif
    }
    if (chb == 0)
    {
        chb = cha;
#ifdef DEBUG
        Serial.println("Chanel B = 0");
#endif
    }
    if (cha == chb)
    {
        if (cha == 0)
        {
            return timeout;
        }
        return cha;
    }
    return (cha + chb) / 2;
}

//Gets encoder output for chanel A
const long Encoder::getChanelA()
{
    long val = getChanel(pinA);
    if (val == 0)
    {
        return timeout;
    }
    return val;
}

//Gets encoder output for chanel B
//Currently I don't think we need chanel B b/c it's just for getting the direction (I think)
const long Encoder::getChanelB()
{
    return getChanel(pinB);
}

const long Encoder::getChanel(int pin)
{
    long encoderOutput = pulseIn(pin, HIGH, timeout);
    // if (encoderOutput <= 0)
    // {
    //     encoderOutput = minVal;
    // }
    return encoderOutput;
}