#pragma once
/** Only responsible for returning the Pulse Width of the "encoder".
 * The encoder actually functions as a tachometer in that it returns
 * square waves. I chose to force it to return the sum of channel A
 * and B for the average so that it would be a larger number and allow
 * for more precise changes.
 */
class Encoder
{
public:
  /**
 * Initialize the encoder by setting the pin modes to input
 */
  Encoder(int _pinA, int _pinB);
  //Gets the sum of chanel A and chanel B for a more precise value
  const long getAvg();
  //Gets the pulse width of chanel A
  const long getChanelA();
  //Gets the pulse width of chanel B
  const long getChanelB();

private:
  //Gets the pulse width of a specific changel
  const long getChanel(int pin);
  //Pins that the encoder is connected to
  const int pinA, pinB;
  //Timeout in case the motors aren't moving
  const static int timeout = 300;
};