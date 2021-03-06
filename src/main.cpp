//#define DEBUG
#define USE_RFID //Uncommenet to use RFID
#include <Arduino.h>
#include <MotorControl.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
#include <SparkFun_UHF_RFID_Reader.h>
#include <Map.h>
#include <Pins.h>

MotorControl control(Motor(rmDir, rmPWM, Encoder(reA, reB)), Motor(lmDir, lmPWM, Encoder(leA, leB)), ProximitySensorArray(tlb, elb, trb, erb, tlf, elf, trf, erf, tf, ef));
SoftwareSerial softSerial(rfidRX, rfidTX);
uint8_t rfidCount = 0, rfidToggleCount = 30, rfidToggleLength = 10, rfidToggleMax = 80;
bool readingRFID = false, running = false, paused = false;

#ifdef USE_RFID
RFID nano;
#endif

//30 should be a decent walking speed.
int speed = 80;
int previousId = -1;
int currentId = -1;
Direction currentDirection = Stopped;
int currentDestination = -1;

Map myMap(8);

#ifdef DEBUG
void printDirection(Direction dir)
{
  switch (dir)
  {
  case North:
    Serial.print("North");
    break;
  case South:
    Serial.print("South");
    break;
  case East:
    Serial.print("East");
    break;
  case West:
    Serial.print("West");
    break;
  case Stopped:
    Serial.print("Stopped");
    break;
  default:
    Serial.print("Other");
    break;
  }
}

void printlnDirection(Direction dir)
{
  printDirection(dir);
  Serial.println();
}
#endif

#ifdef USE_RFID
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

int checkNano()
{
  if (nano.check() == true) //Check to see if any new data has come in from module
  {
#ifdef DEBUG
    Serial.println("Check NANO");
#endif
    byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

    if (responseType == RESPONSE_IS_KEEPALIVE)
    {
      digitalWrite(yellow, LOW);
#ifdef DEBUG
      Serial.println(F("Scanning"));
#endif
    }
    else if (responseType == RESPONSE_IS_TAGFOUND)
    {
      digitalWrite(yellow, HIGH);

#ifdef DEBUG
      Serial.println("FOUND RFID");
#endif
      //If the RSSI is in the valid range
      //May also need to use this for telling it when to stop (when in stopping range)
      if (nano.getTagRSSI())
      {
#ifdef DEBUG
        int id = nano.msg[31];
        Serial.print("Tag ID: ");
        Serial.println(id);
        return id;
#else
        return nano.msg[31];
#endif
      }
#ifdef DEBUG
      else
      {
        Serial.println("Too far away");
      }
    }
    else if (responseType == ERROR_CORRUPT_RESPONSE)
    {
      Serial.println("Bad CRC");
    }
    else
    {
      //Unknown response
      Serial.print("Unknown error");
#endif
    }
  }
  else
  {
    // Serial.println("NOT CHECKED NANO");
  }
  return -1;
}

#endif

void setup()
{
  pinMode(white, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(switch3, INPUT);

  Serial.begin(115200);
  while (!Serial)
    ; //Wait for the serial port to come online

#ifdef USE_RFID
  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
#ifdef DEBUG
    Serial.println(F("Module failed to respond. Please check wiring."));
#endif
    //Freeze!
    while (!digitalRead(stopButton))
    {
      digitalWrite(red, HIGH);
      digitalWrite(white, HIGH);
      digitalWrite(green, HIGH);
      digitalWrite(yellow, HIGH);
      digitalWrite(blue, HIGH);
      delay(300);
      digitalWrite(red, LOW);
      digitalWrite(white, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(blue, LOW);
      delay(300);
    }
    while (1)
      ;
  }

  nano.setRegion(REGION_NORTHAMERICA); //Set to North America

  nano.setReadPower(1500); //5.00 dBm. Higher values may caues USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling
  nano.startReading();
  // if (readRFID)
  // {
  //   nano.startReading(); //Begin scanning for tags
  // }

#endif
  myMap.setup();
  // myMap.print();
}

void loop()
{
  //Switches
  // int s1 = digitalRead(51);
  // int s2 = digitalRead(52);
  // int s3 = digitalRead(53);

  //Map buttons as well so we can have an emergency stop button without reseting the destination
  //Buttons could also be used as a "GO" button
  if (paused)
  {
    digitalWrite(red, HIGH);
    digitalWrite(white, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(blue, HIGH);
    control.pause();
    control.Update();
    if (digitalRead(stopButton))
    {
      delay(500);
      if (digitalRead(stopButton))
      {
        control.SetSpeed(0);
        control.setTurn(Stopped, Stopped);
        paused = false;
        running = true;
      }
    }
    if (digitalRead(goButton))
    {
      paused = false;
      control.resume();
    }
  }
  else if (control.checkStatus() == Stopped)
  {
    if (running || digitalRead(stopButton))
    {
      running = false;
      currentId = -1;
      currentDestination = -1;
      currentDirection = Stopped;
    }
    control.SetSpeed(0);
    myMap.endRoute();
#ifndef DEBUG
    if (currentId == -1)
    {
      digitalWrite(red, HIGH);
      digitalWrite(white, HIGH);
      digitalWrite(green, HIGH);
      digitalWrite(yellow, HIGH);
      digitalWrite(blue, HIGH);

      if (digitalRead(goButton))
      {
        currentId = (digitalRead(51) << 2) + (digitalRead(52) << 1) + digitalRead(53);
        delay(100);
      }
    }
    else if (currentDirection == Stopped)
    {
      digitalWrite(red, HIGH);
      digitalWrite(yellow, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(blue, LOW);
      digitalWrite(white, LOW);
      if (digitalRead(goButton))
      {
        int tempDirection = digitalRead(51) << 2 + digitalRead(52) << 1 + digitalRead(53);
        if (tempDirection < 4)
        {
          currentDirection = (Direction)tempDirection;
          delay(100);
        }
      }
    }
    else if (currentDestination == -1)
    {
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(blue, HIGH);
      digitalWrite(white, HIGH);
      if (digitalRead(goButton))
      {
        currentDestination = digitalRead(51) << 2 + digitalRead(52) << 1 + digitalRead(53);
      }
    }
    else
    {
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(blue, LOW);
      digitalWrite(green, HIGH);
      digitalWrite(white, LOW);
      myMap.setDestination(currentId, currentDestination, currentDirection);
      control.setTurn(currentDirection, currentDirection);
      control.SetSpeed(speed);
      previousId = currentId;
    }

#else
    Serial.print("Enter current location (##): ");
    while (!Serial.available())
      ;
    String temp = Serial.readString();
    int temp1 = temp.charAt(0) - '0';
    int currentId = temp1;
    if (temp.length() > 1 && temp.charAt(1) != '\n')
    {
      currentId = temp1 * 10 + temp.charAt(1) - '0';
    }
    Serial.println(currentId);
    if (currentId >= 0 && currentId < myMap.size)
    {
      Serial.print("Enter current direction: ");
      while (!Serial.available())
        ;
      String dir = Serial.readString();
      dir.toUpperCase();
      Direction currentDirection = Other;
      if (dir == "NORTH" || dir == "N")
      {
        currentDirection = North;
        Serial.println("NORTH");
      }
      else if (dir == "SOUTH" || dir == "S")
      {
        currentDirection = South;
        Serial.println("SOUTH");
      }
      else if (dir == "EAST" || dir == "E")
      {
        currentDirection = East;
        Serial.println("EAST");
      }
      else if (dir == "WEST" || dir == "W")
      {
        currentDirection = West;
        Serial.println("WEST");
      }

      if (currentDirection != Other)
      {
        Serial.print("Enter a destination");
        while (!Serial.available())
          ;
        temp = Serial.readString();
        temp1 = temp.charAt(0) - '0';
        int destId = temp1;
        if (temp.length() > 1 && temp.charAt(1) != '\n')
        {
          destId = temp1 * 10 + temp.charAt(1) - '0';
        }

        Serial.println(destId);
        if (destId > 0 && destId < myMap.size)
        {
          myMap.setDestination(currentId, destId, currentDirection);
          Serial.print("MOVING ");
          printDirection(currentDirection);
          Serial.print(" Turning ");
          printlnDirection(myMap.getDirection(currentId));

          control.setTurn(currentDirection, currentDirection);
          control.SetSpeed(speed);
          previousId = currentId;
        }
      }
    }
#endif
  }
  else if (digitalRead(stopButton))
  {
    paused = true;
  }
  control.Update();
  delay(20);

#ifdef USE_RFID
  if (myMap.hasDestination())
  {
    int id = checkNano();
    if (id != previousId && id > -1)
    {
#ifdef DEBUG
      Serial.println("Starting Tag Logic");
      Serial.print("Previous ID: ");
      Serial.println(previousId);
#endif

      Direction cur = myMap.getDirection(previousId);
      Direction turn = myMap.getDirection(id);
#ifdef DEBUG
      Serial.print("Found RFID: ");
      Serial.print(id);
      Serial.print(" ");
      printlnDirection(turn);

      Serial.print("Change turn: ");
      printDirection(turn);
      Serial.print(" direction: ");
      printlnDirection(cur);
#endif
      control.setTurn(turn, cur);
      if (turn == Stopped)
      {
        myMap.endRoute();
      }
      previousId = id;
    }
#ifdef DEBUG
    else
    {
      if (id == previousId)
      {
        Serial.print("Same ID: ");
        Serial.println(id);
      }
      else
      {
        Serial.println("ID = -1");
      }
    }
#endif
  }
  else
  {
    digitalWrite(yellow, LOW);
#ifdef DEBUG
    Serial.println("No Map Destination");
#endif
  }
#endif
}
