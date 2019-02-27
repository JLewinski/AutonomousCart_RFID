#include "MotorControl.h"

MotorControl::~MotorControl()
{
    delete right;
    delete left;
    delete sensors;
}