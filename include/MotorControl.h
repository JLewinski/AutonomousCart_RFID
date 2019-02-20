#pragma once

#include "Motor.h"

class MotorControl
{
  public:
    MotorControl(int r_pwm, int r_in_1, int r_in_2, int l_pwm, int l_in_1, int l_in_2)
        : right(Motor(r_in_1, r_in_2, r_pwm)), left(l_in_1, l_in_2, l_pwm) {}
    ~MotorControl();

  private:
    const Motor left;
    const Motor right;
};
