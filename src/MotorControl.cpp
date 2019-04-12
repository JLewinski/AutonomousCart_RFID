// #define DEBUG
#include "MotorControl.h"

//Set the speed for both motors
//There will be another method for angle (direction)
void MotorControl::SetSpeed(int spd)
{
    if (spd == 0)
    {
        left.brake();
        right.brake();
    }
    else if (spd >= -maxSpeed && spd <= maxSpeed)
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
        sensors.readAll();
        uint32_t rf = sensors.getRecent(RightFront);
        uint32_t rb = sensors.getRecent(RightBack);
        uint32_t lf = sensors.getRecent(LeftFront);
        uint32_t lb = sensors.getRecent(LeftBack);
        uint32_t f = sensors.getRecent(Front);

#ifdef DEBUG
        Serial.print("rf: ");
        Serial.print(rf);
        Serial.print("   rb: ");
        Serial.print(rb);
        Serial.print("    lf: ");
        Serial.print(lf);
        Serial.print("    lb: ");
        Serial.print(lb);
        Serial.print("   front: ");
        Serial.println(f);
#endif

        if (f <= dangerFront)
        {
            int tempSpeed = speed;
            SetSpeed(0);
            speed = tempSpeed;
            direction = Stopped;
            delay(999);
            return;
        }

        if (direction == Stopped)
        {
            direction = Forward;
            SetSpeed(speed);
        }

        if (rf + lf > hallWidth + safeDistance)
        {
            if (frontSensor)
            {
                frontSensorChoice = sensors.diff(RightFront) > sensors.diff(LeftFront);
                frontSensor = (false);
            }
            if (frontSensorChoice)
            {
                rf = hallWidth - lf;
            }
        }
        else
        {
            frontSensor = (true);
        }

        if (rb + lb > hallWidth + safeDistance)
        {
            if (backSensor)
            {
                backSensorChoice = sensors.diff(RightBack) > sensors.diff(LeftBack);
                backSensor = (false);
            }

            if (backSensorChoice)
            {
                rb = hallWidth - lb;
            }
        }

        if (rf >= ProximitySensorArray::timeOut - safeDistance)
        {
            //If it wasn't already at an intersection
            if (intersection)
            {
                rightOffset++;
                intersection = (false);
            }
        }
        else
        {
            //not at an intersection
            intersection = (true);

            updateOffset(rf, rb);
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
};

void MotorControl::updateOffset(int rf, int rb)
{
    int diff = (rf - desiredDistance);
    int absDiff = abs(diff);
    int absPreviousDistance = abs(previousDistanceDiff);

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
            rightOffset -= offsetInc;
        }
        else if (diff < 0)
        {
            rightOffset += offsetInc;
        }
        else if (previousAngleDiff > 0)
        {
            rightOffset += offsetInc;
        }
        else if (previousAngleDiff < 0)
        {
            rightOffset -= offsetInc;
        }

        previousAngleDiff = diff;
    }
    else if (diff >= dangerDistance)
    {
        rightOffset = -offsetMax;
    }
    else if (diff <= -dangerDistance)
    {
        rightOffset = offsetMax;
    }
    else if ((diff > 0 && previousDistanceDiff < 0) || (diff < 0 && previousDistanceDiff > 0))
    {
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
            rightOffset--;
        }
        else
        {
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
};