#define DEBUG
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

        right.setSpeed(spd + rightOffset);
        left.setSpeed(spd + leftOffset);
    }
};

//Update both motors for the desired speed
void MotorControl::Update()
{
    if (count++ > countMax)
    {
        uint32_t rf = sensors.read(RightFront);
        uint32_t rb = sensors.read(RightBack);
#ifdef DEBUG
        Serial.print("****RF: ");
        Serial.println(rf);
        Serial.print("RB: ");
        Serial.println(rb);
#endif

        int diff = rf - desiredDistance;

        // diff -= rf - desiredDistance;

        if (diff > minDif)
        {
            diff -= minDif;
        }
        else if (diff < 0 - minDif)
        {
            diff += minDif;
        }
        else
        {
            diff = 0;
        }

        if (diff > 5)
        {
            diff = 5;
        }
        else if (diff < -5)
        {
            diff = -5;
        }

        if ((rightOffset > -5 && diff < 0) || (rightOffset < 5 && diff > 0))
        {
            rightOffset += diff;
            leftOffset -= diff;
            right.setSpeed(speed + rightOffset);
            left.setSpeed(speed - rightOffset);
        }
#ifdef DEBUG
        Serial.print("****DIFF: ");
        Serial.println(diff);
        Serial.print("RToff: ");
        Serial.println(rightOffset);
#endif
        count = 0;
    }

    right.update();
    left.update();
}