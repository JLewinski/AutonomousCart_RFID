#define DEBUG
#define DEBUG2
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
// Serial.println("This isn't working");
#ifdef DEBUG
    Serial.print("Turn: ");
    Serial.print(turn);
    Serial.print(" Direction: ");
    Serial.println(currentDirection);
#endif
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
#ifdef DEBUG
        int average = sum / hallHistoryLength;
        Serial.print("Average Hall Width: ");
        Serial.println(average);
        return average;
#else
        return sum / hallHistoryLength;
#endif
    }
    return hallWidthHistory[0];
}

int MotorControl::getDistance(UltrasonicSensor right, UltrasonicSensor left, bool update = true)
{
    int r = sensors.getRecent(right);
    int l = sensors.getRecent(left);
    //

    int width = r + l;
    int hallWidth = defaultHallWidth;
    if (update && (r == ProximitySensorArray::timeOut || l == ProximitySensorArray::timeOut))
    {
#ifdef DEBUG
        Serial.print("TIMEOUT");
        if (r == ProximitySensorArray::timeOut)
            Serial.println(" RIGHT");
        else
            Serial.println(" LEFT");
#endif
        width = 0;
        digitalWrite(red, HIGH);
        if (intersectionFlag < maxIntersection)
            intersectionFlag++;
#ifdef DEBUG
        Serial.print("Intersection Flag: ");
        Serial.println(intersectionFlag);
#endif
    }
    else if (update)
    {
        digitalWrite(red, LOW);
        if (intersectionFlag > intersectionFlagInit + 1)
        {
            intersectionFlag -= 2;
        }
        else if (intersectionFlag > 0)
        {
            intersectionFlag--;
        }
#ifdef DEBUG
        Serial.print("Intersection Flag: ");
        Serial.println(intersectionFlag);
#endif
    }

    if (update)
    {
        hallWidth = getAverageHallWidth(width);
    }
    else
    {
        hallWidth = getAverageHallWidth();
    }

#ifdef DEBUG
    //possibly changed to percentage of hallWidth (within 5%)?
    //Just show when there is something of a discrepency
    if (width > hallWidth + safeDistance)
    {
        digitalWrite(blue, HIGH);
    }
    else if (width < hallWidth - safeDistance)
    {
        digitalWrite(blue, HIGH);
    }
    else
    {
        digitalWrite(blue, LOW);
    }
#endif

    int distanceFromL = hallWidth - l;

    if (l != ProximitySensorArray::timeOut && r != ProximitySensorArray::timeOut)
    {
        return (distanceFromL + r) / 2;
    }
    else if (l == ProximitySensorArray::timeOut)
    {
        return r;
    }
    else
    {
        return distanceFromL;
    }
}

//Update both motors for the desired speed
void MotorControl::Update()
{
    if (speed == 0 || direction == Stopped)
    {
        right.brake();
        left.brake();
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

#ifdef DEBUG2
#ifndef DEBUG
        Serial.print("Front: ");
        Serial.print(frontDistance);
#endif
#endif
#ifdef DEBUG
        Serial.print("Front: ");
        Serial.print(frontDistance);
#endif
        int backDistance = getDistance(RightBack, LeftBack, false);

        updateOffset(frontDistance, backDistance);

#ifdef DEBUG2
#ifndef DEBUG
        Serial.print("   |   RIGHT OFFSET: ");
        Serial.println(rightOffset);
#endif
#endif
#ifdef DEBUG
        Serial.print("   |   RIGHT OFFSET: ");
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
        if (turn == Stopped)
        {
            direction = Stopped;
#ifdef DEBUG
            Serial.println("----STOPPED-----");
#endif
        }
        else if ((direction == North && turn == East) ||
                 (direction == East && turn == South) ||
                 (direction == South && turn == West) ||
                 (direction == West && turn == North))
        {
            leftOffset = offsetMax;
            rightOffset = -turnOffset;
            digitalWrite(green, HIGH);
            digitalWrite(white, LOW);
        }
        else
        {
            rightOffset = turnOffset;
            leftOffset = -offsetMax;
            digitalWrite(green, LOW);
            digitalWrite(white, HIGH);
        }
    }
    if (direction == Stopped)
    {
        leftOffset = 0;
        rightOffset = 0;
        SetSpeed(0);
        digitalWrite(green, LOW);
        digitalWrite(white, LOW);
    }
}

void MotorControl::updateOffset(int rf, int rb)
{
    int diff = (rf - desiredDistance);
    int diffBack = (rb - desiredDistance);
    int absDiff = abs(diff);

    //Dangerously close to wall
    if (diff <= -dangerDistance || diffBack <= -dangerDistance * 1.1)
    {
#ifdef DEBUG2
        if (diff > -dangerDistance)
        {
            Serial.println("Set to max bc of back sensors");
        }
#endif
        rightOffset = offsetMax;
    }
    else if (intersectionFlag > intersectionFlagInit)
    {
#ifdef DEBUG
        Serial.print("INTERSECTION: ");
        Serial.println(intersectionFlag);
#endif
        if (turn == Stopped)
        {
            SetSpeed(0);
            direction = Stopped;
        }
        else
        {
            initiateTurn();
        }
    }
    //not at an intersection
    else
    {
        digitalWrite(green, LOW);
        digitalWrite(white, LOW);
        leftOffset = 0;

        //Dangerously far from wall
        if (diff >= dangerDistance) // || diffBack >= dangerDistance * 1.25)
        {
#ifdef DEBUG2
            if (diff < dangerDistance)
            {
                Serial.println("Set to max bc of back sensors");
            }
#endif
            rightOffset = -offsetMax;
        }
        //Front-right of cart within safe distance from wall
        else if (absDiff <= safeDistance)
        {
            int angleDiff = rf - rb;
            int diffWithPrevious = diff - previousDistanceDiff;
            int changeValue = 1;
            //safe distance is 2750-2250
            if (abs(diff - previousDistanceDiff) > safeDistance)
            {
                changeValue = 3;
            }
            if (diff < previousDistanceDiff)
            {
                rightOffset += changeValue;
            }
            //getting further from wall and too far from wall
            else
            {
                rightOffset -= changeValue;
            }
            if (rightOffset > offsetMax / 3)
            {
                rightOffset = offsetMax / 3;
            }
            else if (rightOffset < -offsetMax / 3)
            {
                rightOffset = -offsetMax / 3;
            }
        }
        //Changed between close and far
        else if ((diff > 0 && previousDistanceDiff < 0) || (diff < 0 && previousDistanceDiff > 0))
        {
#ifdef DEBUG
            Serial.println("RightOffset *= -75%");
#endif
            rightOffset *= -0.75;
        }
        //Just out of the danger zone or done with a turn
        else if (abs(rightOffset) >= offsetMax)
        {
            rightOffset *= 0.3;
        }
        //getting closer to wall and too close to wall
        else if (rf < previousDistance && diff < 0)
        {
            if (diff < -warningDistance)
            {
                rightOffset += 4;
            }
            else
            {
                rightOffset += 2;
            }
        }

        //getting further from wall and too far from wall
        else if (rf > previousDistance && diff > 0)
        {
            if (diff > warningDistance)
            {
                rightOffset -= 4;
            }
            else
            {
                rightOffset -= 2;
            }
        }
        //Prevent over correction
        else if (abs(previousDistance - rf) > safeDistance)
        {
            rightOffset *= -.3;
        }
    }
    previousDistanceDiff = diff;
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