#pragma once
#include <SoftwareSerial.h> //Used for transmitting to the device
#include <SparkFun_UHF_RFID_Reader.h>

class RFIDControl
{
  public:
    RFIDControl(int pinRX, int pinTX, long baudRate);

  private:
    bool setupNano(long baudRate);
    SoftwareSerial sofSerial;
    RFID nano;
};
