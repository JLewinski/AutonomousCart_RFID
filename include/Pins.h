#pragma once

/** File: Direction.h
 * Author: Jacob Lewinski (jdlewinski)
 * Description: Contains a reference to all pins. To make
 * it easy to change pin numbers for specific wire scheme
 */
enum Pins
{
    //Right Motor Dir
    rmDir = 22,
    //Right Motor PWM
    rmPWM = 5,
    //Left Motor Dir
    lmDir = 23,
    //Left Motor PWM
    lmPWM = 6,
    //Right Encoder CH A
    reA = 24,
    //Right Encoder CH B
    reB = 26,
    //Left Encoder CH A
    leA = 25,
    //LEFT ENCODER CH B
    leB = 27,
    //Left Back Trigger
    tlb = 28,
    //Left Back Echo
    elb = 29,
    //Right Back Trigger
    trb = 30,
    //Right Back Echo
    erb = 31,
    //Left Front Trigger
    tlf = 32,
    //Left Front Echo
    elf = 33,
    //Right Front Trigger
    trf = 34,
    //Right Front Echo
    erf = 35,
    //Front Trigger
    tf = 36,
    //Front Echo
    ef = 37,
    //RX for RFID (serial communication)
    rfidRX = 11,
    //TX for RFID (serial communication)
    rfidTX = 3,
    //Switch 1 (from left)
    switch1 = 51,
    //Switch 2 (from left)
    switch2 = 52,
    //Switch 3 (from left)
    switch3 = 53,
    goButton = 50,
    stopButton = 49,
    //LED: safe zone
    white = 40,
    //LED: abnormal hall width
    blue = 41,
    //LED: Turning
    green = 42,
    //LED: RFID Scanned
    yellow = 43,
    //LED: TIMEOUT
    red = 44
};