#pragma onece

#include <Ultrasonic.h>

enum UltrasonicSensor
{
  LeftFront,
  RightFront,
  LeftBack,
  RightBack,
  Front
};

class ProximitySensorArray
{
public:
  //PINS: Echo, left-back-trigger, left-front-trigger, right-back-trigger, right-front-trigger, front-trigger
  ProximitySensorArray(int tlb, int elb, int trb, int erb, int tlf, int elf, int trf, int erf, int tf, int ef);
  ~ProximitySensorArray();

  long read(UltrasonicSensor ultrasonicSensor);

private:
  Ultrasonic leftBack;
  Ultrasonic leftFront;
  Ultrasonic rightBack;
  Ultrasonic rightFront;
  Ultrasonic front;

  long *leftBackValues = new long[5]{0, 0, 0, 0, 0};
  long *leftFrontValues = new long[5]{0, 0, 0, 0, 0};
  long *rightBackValues = new long[5]{0, 0, 0, 0, 0};
  long *rightFrontValues = new long[5]{0, 0, 0, 0, 0};
  long *frontValues = new long[5]{0, 0, 0, 0, 0};
  long t1, t2;
  void shift(long *values);
};