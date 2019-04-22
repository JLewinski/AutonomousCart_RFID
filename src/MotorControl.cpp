//#define DEBUG
#include "MotorControl.h"

bool MotorControl::hasNextTurn()
{
    return turn != direction;
}

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

void MotorControl::setTurn(Direction d)
{
    turn = d;
}

int MotorControl::getAverageHallWidth(int currentWidth = 0)
{
    if (currentWidth != 0)
    {
        hallWidthHistory[hallIndex++] = currentWidth;
    }

    if (hallIndex == hallHistoryLength)
    {
        hallIndex = 0;
    }

    if (hallWidthHistory[hallIndex])
    {
        long sum = 0;
        for (int i = 0; i < hallHistoryLength; i++)
        {
            sum += hallWidthHistory[i];
        }
        return sum / hallHistoryLength;
    }
    return hallWidthHistory[0];
}

int MotorControl::getDistance(UltrasonicSensor right, UltrasonicSensor left)
{
    int r = sensors.getRecent(right);
    int l = sensors.getRecent(left);
    //

    int width = r + l;
    int hallWidth = defaultHallWidth;
    if (r == ProximitySensorArray::timeOut || l == ProximitySensorArray::timeOut)
    {
        width = 0;
    }

    hallWidth = getAverageHallWidth(width);
#ifdef DEBUG
    Serial.print("Avg hall width: ");
    Serial.println(hallWidth);
#endif

    //possibly changed to percentage of hallWidth (within 5%)?
    //Just show when there is something of a discrepency
    if (width > hallWidth + safeDistance || width < hallWidth - safeDistance)
    {
        digitalWrite(41, HIGH);
    }
    else
    {
        digitalWrite(41, LOW);
    }

    if (l <= dangerDistance || (hallWidth - l) < r)
    {
        return hallWidth - l;
    }
    else
    {
        return r;
    }
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
            blocked = (true);
            delay(999);
            return;
        }

        if (blocked)
        {
            blocked = (false);
            SetSpeed(speed);
        }

        int frontDistance = getDistance(RightFront, LeftFront);
#ifdef DEBUG
        Serial.print("Front: ");
        Serial.println(frontDistance);
#endif
        if (frontDistance >= ProximitySensorArray::timeOut - safeDistance)
        {
            //If it wasn't already at an intersection
            if (intersection && turn != direction)
            {
                intersection = (false);
            }
            else
            {
                if (turn == Stopped)
                {
                    SetSpeed(0);
                }
                else
                {
                    initiateTurn();
                }
            }
            digitalWrite(40, HIGH);
        }
        else
        {
            digitalWrite(40, LOW);
            //not at an intersection
            intersection = (true);
            int backDistance = getDistance(RightBack, LeftBack);

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

void MotorControl::initiateTurn()
{
    if (direction != turn)
    {
        rightOffset = offsetMax;
        switch (direction)
        {
        case North:
            if (turn == East)
            {
                rightOffset *= -1;
            }
            break;
        case East:
            if (turn == South)
            {
                rightOffset *= -1;
            }
            break;
        case South:
            if (turn == West)
            {
                rightOffset *= -1;
            }
            break;
        case West:
            if (turn == North)
            {
                rightOffset *= -1;
            }
            break;
        case Stopped:
            SetSpeed(0);
            break;
        case Other:
        default:
            //do nothing
            break;
        }
    }
}

void MotorControl::updateOffset(int rf, int rb)
{

    int diff = (rf - desiredDistance);
    int absDiff = abs(diff);
    int absPreviousDistance = abs(previousDistanceDiff);

    //Front-right of cart within safe distance from wall
    if (absDiff <= safeDistance)
    {
        previousDistanceDiff = diff;
        diff = rf - rb;
        absDiff = abs(diff);
        int offsetInc = 5;
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
    //Front-right not within safe distance of wall
    else
    {
        //Front-right of cart dangerously far from wall
        if (diff >= dangerDistance)
        {
            rightOffset = -offsetMax;
        }
        //Front-right of cart dangerously close to wall
        else if (diff <= -dangerDistance)
        {
            rightOffset = offsetMax;
        }
        //It was too far one way and now is too far the other way (I doubt it ever gets here)
        else if ((diff > 0 && previousDistanceDiff < 0) || (diff < 0 && previousDistanceDiff > 0))
        {
            rightOffset *= -0.75;
        }
        //getting closer to wall
        else if (absDiff < absPreviousDistance)
        {
            if (abs(rightOffset) == offsetMax)
            {
                rightOffset *= 0.7;
            }
        }
        //getting further from wall
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
        previousDistanceDiff = diff;
    }

    //Make sure the offset isn't greater than the max
    if (rightOffset > offsetMax)
    {
        rightOffset = offsetMax;
    }
    else if (rightOffset < -offsetMax)
    {
        rightOffset = -offsetMax;
    }
};