//#define DEBUG
#include "MotorControl.h"
#include "Pins.h"

MotorControl::MotorControl(Motor _right, Motor _left, ProximitySensorArray _sensors) : left(_left), right(_right), sensors(_sensors)
{
    direction = Stopped;
    turn = Stopped;
    SetSpeed(0);
}

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

void MotorControl::setTurn(Direction nextTurn, Direction currentDirection)
{
    turn = nextTurn;
    direction = currentDirection;
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
        digitalWrite(red, HIGH);
    }
    else
    {
        digitalWrite(red, LOW);
    }

    hallWidth = getAverageHallWidth(width);

    //possibly changed to percentage of hallWidth (within 5%)?
    //Just show when there is something of a discrepency
    if (width > hallWidth + safeDistance || width < hallWidth - safeDistance)
    {
        digitalWrite(blue, HIGH);
    }
    else
    {
        digitalWrite(blue, LOW);
    }

    if (l != ProximitySensorArray::timeOut && (l <= dangerDistance || (hallWidth - l) < r))
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
    if (speed == 0)
    {
        return;
    }
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
            digitalWrite(green, HIGH);
        }
        else
        {
            digitalWrite(green, LOW);
            //not at an intersection
            intersection = (true);
            int backDistance = getDistance(RightBack, LeftBack);

            updateOffset(frontDistance, backDistance);
            leftOffset = 0;
        }

#ifdef DEBUG
        Serial.print("RIGHT OFFSET: ");
        Serial.println(rightOffset);
#endif
        right.setSpeed(speed + rightOffset);
        left.setSpeed(speed + leftOffset);
        count = 0;
    }

    right.update();
    left.update();
};

void MotorControl::initiateTurn()
{
    if (direction != turn)
    {
        int origOffset = rightOffset;
        rightOffset = turnOffset;
        leftOffset = -turnOffset;
        switch (direction)
        {
        case North:
            if (turn == East)
            {
                leftOffset = 0;
                rightOffset = -turnOffset;
            }
            break;
        case East:
            if (turn == South)
            {
                leftOffset = 0;
                rightOffset = -turnOffset;
            }
            break;
        case South:
            if (turn == West)
            {
                leftOffset = 0;
                rightOffset = -turnOffset;
            }
            break;
        case West:
            if (turn == North)
            {
                leftOffset = 0;
                rightOffset = -turnOffset;
            }
            break;
        case Stopped:
            SetSpeed(0);
            leftOffset = 0;
            rightOffset = 0;
            break;
        case Other:
        default:
            leftOffset = 0;
            rightOffset = origOffset;
            break;
        }
    }
}

void MotorControl::updateOffset(int rf, int rb)
{
    int diff = (rf - desiredDistance);
    int absDiff = abs(diff);

    //Front-right of cart within safe distance from wall
    if (absDiff <= safeDistance)
    {
        digitalWrite(white, HIGH);
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
        digitalWrite(white, LOW);
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
#ifdef DEBUG
            Serial.println("RightOffset *= -75%");
#endif
            rightOffset *= -0.75;
        }
        //Left the danger zone or done with a turn
        if (abs(rightOffset) >= offsetMax)
        {
            rightOffset *= 0.6;
        }
        //getting closer to wall and too close to wall
        else if (rf < previousDistance && diff < 0)
        {
            rightOffset += 2;
        }
        //getting further from wall and too far from wall
        else if (rf > previousDistance && diff > 0)
        {
            rightOffset -= 2;
        }
        previousDistanceDiff = diff;
    }

    previousDistance = rf;

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

Direction MotorControl::checkStatus()
{
    return direction;
}