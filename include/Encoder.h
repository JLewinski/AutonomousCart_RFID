#pragma once

class Encoder
{
public:
  Encoder(int _pinA, int _pinB);
  const long getAvg();
  const long getChanelA();
  const long getChanelB();

private:
  const long getChanel(int pin);
  const int pinA, pinB;
  const static int timeout = 300;
};