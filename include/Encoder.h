#pragma once

class Encoder
{
public:
  Encoder(int _pinA, int _pinB);
  const long getChanelA();
  const long getChanelB();

private:
  const int pinA, pinB;
};