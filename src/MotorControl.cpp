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
        int diff = (rf - desiredDistance);
        int absDiff = abs(diff);
        int absPreviousDistance = abs(previousDistanceDiff);
#ifdef DEBUG
        Serial.print("****RF: ");
        Serial.println(rf);
        Serial.print("RB: ");
        Serial.println(rb);
        Serial.print("diff: ");
        Serial.println(diff);
#endif

        if (absDiff <= safeDistance)
        {
            previousDistanceDiff = diff;
            diff = rf - rb;
            absDiff = abs(diff);
            int offsetInc = 2;
            if (absDiff <= safeAngle)
            {
                offsetInc = 1;
            }

            if (diff > 0)
            {
#ifdef DEBUG
                Serial.println("Decrease");
#endif
                rightOffset -= offsetInc;
            }
            else if (diff < 0)
            {
#ifdef DEBUG
                Serial.println("Increase");
#endif
                rightOffset += offsetInc;
            }
            else if (previousAngleDiff > 0)
            {
#ifdef DEBUG
                Serial.println("Increase");
#endif
                rightOffset += offsetInc;
            }
            else if (previousAngleDiff < 0)
            {
#ifdef DEBUG
                Serial.println("Decrease");
#endif
                rightOffset -= offsetInc;
            }

            previousAngleDiff = diff;
        }
        else if (diff >= dangerDistance)
        {
#ifdef DEBUG
            Serial.println("-Max-");
#endif
            rightOffset = -offsetMax;
        }
        else if (diff <= -dangerDistance)
        {
#ifdef DEBUG
            Serial.println("+Max+");
#endif
            rightOffset = offsetMax;
        }
        else if ((diff > 0 && previousDistanceDiff < 0) || (diff < 0 && previousDistanceDiff > 0))
        {
#ifdef DEBUG
            Serial.println("-0.75");
#endif
            rightOffset *= -0.75;
        }
        else if (absDiff < absPreviousDistance)
        {
            if (abs(rightOffset) == offsetMax)
            {
                rightOffset *= 0.7;
            }
        }
        else
        {
            if (diff > 0)
            {
#ifdef DEBUG
                Serial.println("Decrease");
#endif
                rightOffset--;
            }
            else
            {
#ifdef DEBUG
                Serial.println("Increase");
#endif
                rightOffset++;
            }
        }

        if (rightOffset > offsetMax)
        {
            rightOffset = offsetMax;
        }
        else if (rightOffset < -offsetMax)
        {
            rightOffset = -offsetMax;
        }
#ifdef DEBUG
        Serial.print("RIGHT OFFSET: ");
        Serial.println(rightOffset);
#endif
        right.setSpeed(speed + rightOffset);
        count = 0;
    }

    right.update();
    left.update();
}