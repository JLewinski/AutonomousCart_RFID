#include <Arduino.h>
#include <MotorControl.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
#include <SparkFun_UHF_RFID_Reader.h>

MotorControl *control;
SoftwareSerial softSerial(11, 3); //RX, TX
RFID nano;

//28 should be a decent walking speed.
int speed = 28;

//Testing Variables
int incriment = 1;
int max = 35;
int min = 9;
int up = 1;
int down = -1;

boolean setupNano(long baudRate)
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

void checkNano()
{
  if (nano.check() == true) //Check to see if any new data has come in from module
  {
    byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

    if (responseType == RESPONSE_IS_KEEPALIVE)
    {
      Serial.println(F("Scanning"));
    }
    else if (responseType == RESPONSE_IS_TAGFOUND)
    {
      //If we have a full record we can pull out the fun bits
      int rssi = nano.getTagRSSI(); //Get the RSSI for this tag read

      long freq = nano.getTagFreq(); //Get the frequency this tag was detected at

      long timeStamp = nano.getTagTimestamp(); //Get the time this was read, (ms) since last keep-alive message

      byte tagEPCBytes = nano.getTagEPCBytes(); //Get the number of bytes of EPC from response

      Serial.print(F(" rssi["));
      Serial.print(rssi);
      Serial.print(F("]"));

      Serial.print(F(" freq["));
      Serial.print(freq);
      Serial.print(F("]"));

      Serial.print(F(" time["));
      Serial.print(timeStamp);
      Serial.print(F("]"));

      //Print EPC bytes, this is a subsection of bytes from the response/msg array
      Serial.print(F(" epc["));
      for (byte x = 0; x < tagEPCBytes; x++)
      {
        if (nano.msg[31 + x] < 0x10)
          Serial.print(F("0")); //Pretty print
        Serial.print(nano.msg[31 + x], HEX);
        Serial.print(F(" "));
      }
      Serial.print(F("]"));

      Serial.println();
    }
    else if (responseType == ERROR_CORRUPT_RESPONSE)
    {
      Serial.println("Bad CRC");
    }
    else
    {
      //Unknown response
      Serial.print("Unknown error");
    }
  }
}

void setup()
{
  Motor *right = new Motor(22, 5, new Encoder(24, 26));
  Motor *left = new Motor(23, 6, new Encoder(25, 27));
  ProximitySensorArray *sensors = new ProximitySensorArray(28, 29, 30, 31, 32, 33, 34, 35, 36, 37);
  control = new MotorControl(right, left, sensors);
  control->SetSpeed(speed);

  Serial.begin(115200);
  while (!Serial)
    ; //Wait for the serial port to come online

  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    while (1)
      ; //Freeze!
  }

  nano.setRegion(REGION_NORTHAMERICA); //Set to North America

  nano.setReadPower(1500); //5.00 dBm. Higher values may caues USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

  //Dont wait for user input
  // Serial.println(F("Press a key to begin scanning for tags."));
  // while (!Serial.available())
  //   ;            //Wait for user to send a character
  // Serial.read(); //Throw away the user's character

  nano.startReading(); //Begin scanning for tags
}

//Slightly increments and decrements the speed to test encoder output for differnet speeds.
//This may also be good to use to test different speeds for a optimum walking speed.
void test()
{
  speed += incriment;

  //Make sure the speed only gets set within the parameters
  if (speed <= max && speed >= min)
  {
    control->SetSpeed(speed);
    Serial.print("Speed: ");
    Serial.println(speed);
  }

  //Start decrementing if speed is above max
  if (speed > max)
  {
    incriment = down;
  }

  //Start incrementing if speed is lower than min
  if (speed < min)
  {
    incriment = up;
  }

  //Hold for a set amount of time
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
