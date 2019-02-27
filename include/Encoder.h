#pragma once

class Encoder
{
  public:
    Encoder(int _pinA, int _pinB);
    ~Encoder();

  private:
    int pinA, pinB;
}