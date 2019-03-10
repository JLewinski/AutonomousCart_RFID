#pragma once

class Encoder
{
public:
  Encoder(int _pinA, int _pinB);
  ~Encoder();
  const long getChanelA();
  const long getChanelB();
private:
  const int pinA, pinB;
};