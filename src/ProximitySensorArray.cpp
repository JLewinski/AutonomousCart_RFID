#include <ProximitySensorArray.h>
#include <Arduino.h>

ProximitySensorArray::ProximitySensorArray(int tlb, int elb, int trb, int erb, int tlf, int elf, int trf, int erf, int tf, int ef)
    : sensors({Ultrasonic(tf, ef, timeOut), Ultrasonic(tlf, elf, timeOut), Ultrasonic(trf, erf, timeOut), Ultrasonic(tlb, elb, timeOut), Ultrasonic(trb, erb, timeOut)})
{
}

long ProximitySensorArray::read(UltrasonicSensor ultrasonicSensor)
{
    for (int i = historySize - 1; i >= 1; i--)
    {
        history[ultrasonicSensor][i] = history[ultrasonicSensor][i - 1];
    }

    history[ultrasonicSensor][0] = sensors[ultrasonicSensor].Timing();

    return history[ultrasonicSensor][0];
}

void ProximitySensorArray::readAll()
{
    for (int i = 0; i < numSensors; i++)
    {
        read((UltrasonicSensor)i);
    }
}

long ProximitySensorArray::diff(UltrasonicSensor ultrasonicSensor)
{
    return abs(history[ultrasonicSensor][0] - history[ultrasonicSensor][1]);
}

long ProximitySensorArray::avg(UltrasonicSensor ultrasonicSensor)
{
    long avg = 0;
    for (int i = 0; i < historySize; i++)
    {
        avg += history[ultrasonicSensor][i];
    }
    return avg / historySize;
}

long *ProximitySensorArray::getHistory(UltrasonicSensor ultrasonicSensor)
{
    return history[ultrasonicSensor];
}

long ProximitySensorArray::getRecent(UltrasonicSensor ultrasonicSensor)
{
    return history[ultrasonicSensor][0];
}