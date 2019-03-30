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

void MotorControl::QuiteDif(int *tempDif)
{
    if (*tempDif > minDif)
    {
        *tempDif -= minDif;
    }
    else if (*tempDif < 0 - minDif)
    {
        *tempDif += minDif;
    }
    else
    {
        *tempDif = 0;
    }
}

//Update both motors for the desired speed
void MotorControl::Update()
{
    if (count++ > countMax)
    {
        uint32_t rf = sensors.read(RightFront);
        uint32_t rb = sensors.read(RightBack);
#ifdef DEBUG
        // Serial.print("****RF: ");
        // Serial.println(rf);
        // Serial.print("RB: ");
        // Serial.println(rb);
#endif

        int diff = (rf - desiredDistance) / 50;
        QuiteDif(&diff);
        if (diff == 0)
        {
            diff = rf - rb;
            // QuiteDif(&diff);
        }

        if (diff > maxDif)
        {
            diff = maxDif;
        }
        else if (diff < -maxDif)
        {
            diff = -maxDif;
        }

        if (toggle)
        {
            if ((rightOffset > -offsetMax && diff < 0) || (rightOffset < offsetMax && diff > 0))
            {
                rightOffset += diff;
                right.setSpeed(speed + rightOffset);
                toggle = false;
#ifdef DEBUG
                Serial.print("RToff: ");
                Serial.println(rightOffset);
#endif
            }
        }
        else if ((leftOffset < offsetMax && diff < 0) || (leftOffset > -offsetMax && diff > 0))
        {
            leftOffset -= diff;
            left.setSpeed(speed + leftOffset);
            toggle = true;
#ifdef DEBUG
            Serial.print("LFoff: ");
            Serial.println(leftOffset);
#endif
        }
#ifdef DEBUG
        // Serial.print("****DIFF: ");
        // Serial.println(diff);
#endif
        count = 0;
    }

    right.update();
    left.update();
}