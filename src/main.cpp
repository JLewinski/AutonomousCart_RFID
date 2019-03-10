#include <Arduino.h>
#include <MotorControl.h>

MotorControl *control;

//28 should be a decent walking speed.
int speed = 28;
int incriment = 1;
int max = 35;
int min = 9;
int up = 1;
int down = -1;

void setup()
{
  Motor *right = new Motor(22, 5, new Encoder(24, 26));
  Motor *left = new Motor(23, 6, new Encoder(25, 27));
  ProximitySensorArray *sensors = new ProximitySensorArray(28, 29, 30, 31, 32, 33, 34, 35, 36, 37);
  control = new MotorControl(right, left, sensors);
  control->SetSpeed(speed);
  Serial.begin(9600);
}

//Slightly increments and decrements the speed to test encoder output for differnet speeds.
//This may also be good to use to test different speeds for a optimum walking speed.
void test()
{
  speed += incriment;

  if (speed <= max && speed >= min)
  {
    control->SetSpeed(speed);
    Serial.print("Speed: ");
    Serial.println(speed);
  }
  if (speed > max)
  {
    incriment = down;
  }
  if (speed < min)
  {
    incriment = up;
  }
  for (int i = 0; i < 50; i++)
  {
    control->Update();
    delay(50);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  control->Update();

  // test();
}
