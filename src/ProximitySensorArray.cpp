#include <ProximitySensorArray.h>
#include <Arduino.h>

ProximitySensorArray::ProximitySensorArray(int tlb, int elb, int trb, int erb, int tlf, int elf, int trf, int erf, int tf, int ef)
    : sensors({Ultrasonic(tf, ef, 15000), Ultrasonic(tlf, elf, 15000), Ultrasonic(trf, erf, 15000), Ultrasonic(tlb, elb, 15000), Ultrasonic(trb, erb, 15000)})
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