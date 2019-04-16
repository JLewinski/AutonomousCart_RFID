#define DEBUG
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

int MotorControl::getDistance(UltrasonicSensor right, UltrasonicSensor left, bool &sensor, bool &choice)
{
    int r = sensors.getRecent(right);
    int l = sensors.getRecent(left);
    int diffR = sensors.diff(right);
    int diffL = sensors.diff(left);
    int width = r + l;

    if (width > hallWidth + safeDistance || width < hallWidth - safeDistance)
    {
        digitalWrite(41, HIGH);
        if (r <= dangerDistance)
        {
            return r;
        }
        else if (l <= dangerDistance)
        {
            return hallWidth - l;
        }

        if (sensor)
        {
            choice = sensors.diff(right) > sensors.diff(left);
            sensor = (false);
        }
        if (choice)
        {
            return hallWidth - l;
        }
    }
    else
    {
        digitalWrite(41, LOW);
        sensor = (true);
    }
    return r;
}

//Update both motors for the desired speed
void MotorControl::Update()
{
    if (count++ > countMax)
    {
        sensors.readAll();
#ifdef DEBUG
        int rf = sensors.getRecent(RightFront);
        int rb = sensors.getRecent(RightBack);
        int lf = sensors.getRecent(LeftFront);
        int lb = sensors.getRecent(LeftBack);
        int f = sensors.getRecent(Front);

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

        if (sensors.getRecent(Front) <= dangerFront)
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

        int frontDistance = getDistance(RightFront, LeftFront, frontSensor, frontSensorChoice);
        if (frontDistance >= ProximitySensorArray::timeOut - safeDistance)
        {
            //If it wasn't already at an intersection
            if (intersection)
            {
                rightOffset++;
                intersection = (false);
                digitalWrite(40, HIGH);
            }
        }
        else
        {
            digitalWrite(40, LOW);
            //not at an intersection
            intersection = (true);
            int backDistance = getDistance(RightBack, LeftBack, backSensor, backSensorChoice);

            updateOffset(frontDistance, backDistance);
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