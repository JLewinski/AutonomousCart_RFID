#include <Ultrasonic.h>

//Ability to stronly type the sensors
enum UltrasonicSensor
{
  Front,
  LeftFront,
  RightFront,
  LeftBack,
  RightBack
};

/**
 * Initializes the proximity sensors using a class from a third party.
 * It keeps a track of the 5 previous values from the proximity sensor.
 * It stores all the proximity sensors in an array (as the name implies)
 * and provides various functions to help the user read from the proximity
 * sensors.
 */
class ProximitySensorArray
{
public:
  //PINS: Echo, left-back-trigger, left-front-trigger, right-back-trigger, right-front-trigger, front-trigger
  ProximitySensorArray(int tlb, int elb, int trb, int erb, int tlf, int elf, int trf, int erf, int tf, int ef);
  //Reads a specific proximity sensor and updates the history of that sensor
  long read(UltrasonicSensor ultrasonicSensor);
  //Reads each sensor individually and updates all history
  void readAll();
  //Gets the difference between the two most recent values of a specific sensor
  long diff(UltrasonicSensor ultrasonicSensor);
  //Gets the average value of all the values in history for a specific sensor
  long avg(UltrasonicSensor ultrasonicSensor);
  //Gets the history of a specific sensor
  long *getHistory(UltrasonicSensor ultrasonicSensor);
  //Gets the most recent value of a specific sensor
  long getRecent(UltrasonicSensor ultrasonicSensor);

  //The size of the history
  const static int historySize = 5;
  //The number of sensors in the array
  const static int numSensors = 5;
  //The amount of ticks before stopping the read of the sensors
  const static long timeOut = 15000;

private:
  Ultrasonic sensors[5];

  //Holds the last 5 values read for all 5 sensors
  long history[5][5];
  //Used to shift the history to the left to make room for a new value
  //I just realized would be better to use a circular buffer instead.
  void shift(long *values);
};