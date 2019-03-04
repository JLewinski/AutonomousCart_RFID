#include "MotorControl.h"

MotorControl::~MotorControl()
{
    delete right;
    delete left;
    delete sensors;
};

void MotorControl::SetSpeed(int spd)
{
    if (spd >= -255 && spd <= 255)
    {
        if (spd < 0)
        {
            right->clockwise();
            left->counterClockwise();
        }
        else
        {
            left->clockwise();
            right->counterClockwise();
        }

        speed = spd;

        right->setSpeed(spd);
        left->setSpeed(spd);
    }
};

void MotorControl::Update()
{
    right->update();
    left->update();
}