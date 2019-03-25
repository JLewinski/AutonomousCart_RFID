#include <ProximitySensorArray.h>
#include <Arduino.h>

ProximitySensorArray::ProximitySensorArray(int tlb, int elb, int trb, int erb, int tlf, int elf, int trf, int erf, int tf, int ef)
    : sensors({Ultrasonic(tf, ef), Ultrasonic(tlf, elf), Ultrasonic(trf, erf), Ultrasonic(tlb, elb), Ultrasonic(trb, erb)})
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