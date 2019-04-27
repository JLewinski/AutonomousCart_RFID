#pragma once

#include <Encoder.h>

enum MotorStatus
{
  Clockwise = 0,
  CounterClockwise = 1,
  Stop = 2
};

/**
 * The motor contains an encoder. It allows an outside
 * source to set the desired encoder output and then uses
 * a proportional algorithm to change the PWM value it sends
 * every time it is told to update. Similarly to the motor
 * control, this could be improved by using a PID algorithm
 * however because it is updating at such a high frequency it
 * is not as necessary.
 */
class Motor
{
public:
  //Initializes the pwm pin to an output (analog) and the
  //direction pin to be an output.
  Motor(int _dir, int _pwm, Encoder _encoder);
  //Sets the motor to move clockwise
  void clockwise();
  //Sets the motor to move counter clockwise.
  void counterClockwise();
  //Sets the desired encoder output of the motor based on a speed.
  //It limits it to a max speed.
  void setSpeed(int _speed);
  //Sets the speed to 0
  void brake();
  //Updates the pwm value based on encoder output
  void update();

private:
  const int dir, pwm, maxSpeed = 110;
  Encoder encoder;
  int desiredEncoderOutput = 0;
  int encoderOutput = 0;
  int pwmValue = 0;
  MotorStatus status = Stop;
};
