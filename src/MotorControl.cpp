#include "MotorControl.h"

//Set the speed for both motors
//There will be another method for angle (direction)
void MotorControl::SetSpeed(int spd)
{
    if (spd >= -255 && spd <= 255)
    {
        if (spd < 0)
        {
            right.clockwise();
            left.counterClockwise();
        }
        else
        {
            left.clockwise();
            right.counterClockwise();
        }

        speed = spd;

        right.setSpeed(spd);
        left.setSpeed(spd);
    }
};

//Update both motors for the desired speed
void MotorControl::Update()
{
    right.update();
    left.update();
}