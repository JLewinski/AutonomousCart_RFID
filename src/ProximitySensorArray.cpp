#include <ProximitySensorArray.h>
#include <Arduino.h>

ProximitySensorArray::ProximitySensorArray(int tlb, int elb, int trb, int erb, int tlf, int elf, int trf, int erf, int tf, int ef)
    : leftBack(Ultrasonic(tlb, elb)), rightBack(Ultrasonic(trb, erb)), leftFront(Ultrasonic(tlf, elf)), rightFront(Ultrasonic(trf, erf)), front(Ultrasonic(tf, ef))
{
}

ProximitySensorArray::~ProximitySensorArray()
{
    //Don't need to do this because they aren't dynamically allocated?
    // delete[] leftBackValues;
    // delete[] leftFrontValues;
    // delete[] rightBackValues;
    // delete[] rightFrontValues;
    // delete[] frontValues;
}

long ProximitySensorArray::read(UltrasonicSensor ultrasonicSensor)
{
    long value;
    long *valuesArray;
    switch (ultrasonicSensor)
    {
    case Front:
        value = front.Timing();
        valuesArray = frontValues;
        break;
    case LeftFront:
        value = leftFront.Timing();
        valuesArray = leftFrontValues;
        break;
    case RightFront:
        value = rightFront.Timing();
        valuesArray = rightFrontValues;
        break;
    case LeftBack:
        value = leftBack.Timing();
        valuesArray = leftBackValues;
        break;
    case RightBack:
        value = rightBack.Timing();
        valuesArray = rightBackValues;
        break;
    }
    shift(valuesArray);
    valuesArray[0] = value;
    return value;
}

void ProximitySensorArray::shift(long *values)
{
    for (int i = 4; i >= 1; i--)
    {
        values[i] = values[i - 1];
    }
}