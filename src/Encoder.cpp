#include <Encoder.h>
#include <Arduino.h>

Encoder::Encoder(int _pinA, int _pinB) : pinA(_pinA), pinB(_pinB)
{
}

const long Encoder::getChanelA()
{
    //Might have to use a for loop to add up some of the pulses:
    // long val = 0;
    // for(int i = 0; i < 5; i++){
    //     val += pulseIn(pinA, HIGH, 100);
    // }
    long val = pulseIn(pinA, HIGH, 100);
    Serial.print("Pin");
    Serial.print(pinA);
    Serial.print(": ");
    Serial.println(val);

    // int readVal = digitalRead(pinA);

    // while (readVal)
    //     readVal = digitalRead(pinA);
    // while (!readVal)
    //     readVal = digitalRead(pinA);

    // long ticks = micros();

    // while (readVal)
    //     readVal = digitalRead(pinA);

    // ticks = micros() - ticks;

    // Serial.print("Pin");
    // Serial.print(pinA);
    // Serial.print(" ticks: ");
    // Serial.println(ticks);
    return val;
}

const long Encoder::getChanelB()
{
    return pulseIn(pinB, HIGH);
}