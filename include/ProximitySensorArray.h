#pragma onece

class ProximitySensorArray
{
  public:
    //PINS: Echo, left-back-trigger, left-front-trigger, right-back-trigger, right-front-trigger, front-trigger
    ProximitySensorArray(int _echo, int lb, int lf, int rb, int rf, int f);
    ~ProximitySensorArray();

    int *read();

  private:
    const int echo;
    const int leftBack;
    const int leftFront;
    const int rightBack;
    const int rightFront;
    const int front;
    long leftBackValues[5] = {0, 0, 0, 0, 0};
    long leftFrontValues[5] = {0, 0, 0, 0, 0};
    long rightBackValues[5] = {0, 0, 0, 0, 0};
    long rightFrontValues[5] = {0, 0, 0, 0, 0};
    long frontValues[5] = {0, 0, 0, 0, 0};
    long t1, t2;
    const long read(int pin);
    void shift(long *values, int pin);
}