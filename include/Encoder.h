#pragma once

class Encoder
{
public:
  Encoder(int _pinA, int _pinB);
  const long getChanelA();
  const long getChanelB();

private:
  const long getChanel(int pin);
  const int pinA, pinB, minVal = 90, timeout = 100;
};