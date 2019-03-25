#pragma onece

#include <Ultrasonic.h>

enum UltrasonicSensor
{
  Front,
  LeftFront,
  RightFront,
  LeftBack,
  RightBack
};

class ProximitySensorArray
{
public:
  //PINS: Echo, left-back-trigger, left-front-trigger, right-back-trigger, right-front-trigger, front-trigger
  ProximitySensorArray(int tlb, int elb, int trb, int erb, int tlf, int elf, int trf, int erf, int tf, int ef);
  long read(UltrasonicSensor ultrasonicSensor);

private:
  const int historySize = 5;
  const int numSensors = 5;
  Ultrasonic sensors[5];

  //Front, LeftFront, RightFront, LeftBack, RightBack
  long history[5][5];
  long t1, t2;
  void shift(long *values);
};