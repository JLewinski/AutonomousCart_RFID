#include "RFIDControl.h"

RFIDControl::RFIDControl(int pinRX, int pinTX, long baudRate) :sofSerial()
{
    setupNano(baudRate);
}

boolean RFIDControl::setupNano(long baudRate)
{
    nano.begin(softSerial); //Tell the library to communicate over software serial port

    //Test to see if we are already connected to a module
    //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
    softSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
    while (softSerial.isListening() == false)
        ; //Wait for port to open

    //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
    while (softSerial.available())
        softSerial.read();

    nano.getVersion();

    if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
    {
        //This happens if the baud rate is correct but the module is doing a continuous read
        nano.stopReading();

        Serial.println(F("Module continuously reading. Asking it to stop..."));

        delay(1500);
    }
    else
    {
        //The module did not respond so assume it's just been powered on and communicating at 115200bps
        softSerial.begin(115200); //Start software serial at 115200

        nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

        softSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate

        delay(250);
    }

    //Test the connection
    nano.getVersion();
    if (nano.msg[0] != ALL_GOOD)
        return (false); //Something is not right

    //The M6E has these settings no matter what
    nano.setTagProtocol(); //Set protocol to GEN2

    nano.setAntennaPort(); //Set TX/RX antenna ports to 1

    return (true); //We are ready to rock
}