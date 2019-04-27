# AutonomousCart_RFID
This is a Spring 2019 Senior Design Project at Auburn University

## Getting started:

1. Download and install [VS Code](https://code.visualstudio.com/).
2. Install the [PlatformIO IDE](https://docs.platformio.org/en/latest/ide/vscode.html) extension for VS Code

3. You will need to add 2 libraries to PlatformIO:
  1. [Ultrasonic-HC-SR04 by Bruno Peixoto](https://github.com/brunolnetto/Ultrasonic-HC-SR04?utm_source=platformio&utm_medium=piohome)
  2. [SparkFun Simultaneous RFID Tag Reader Library by SparkFun Electronics](https://github.com/sparkfun/SparkFun_Simultaneous_RFID_Tag_Reader_Library?utm_source=platformio&utm_medium=piohome)

4. Clone this repo into VS Code

NOTE: If you want to commit directly to the repo you will have to get a Github account and I will have to set you up and show you how I want you to do it.

## Code Specifications

### main.cpp

This file sets up the system and functions as the brains of the system. It enables the RFID module to continuously read. It initiates the map and motor control. It allows for the user to interface with the system via switches, buttons, and LEDs. It Uses the input from the user to interface with the map and motor control. It also connects the motor control with the map by taking input from the map and making decisions for the motor control.

The system could be greatly improved by using an operating system to control everything instead of just using a simple while loop. This would make it a lot easier to manage different parts of the system and make it more efficient.

### Motor Control

The motor control class holds 2 motors and an array of proximity sensors. It is currently built to allow an outside source to control which direction it turns and how fast it is going. It also has the potential to allow an outside source to dictate when to turn, but for this project does not use that feature.

The bulk of the processing for this motor control happens when updating it. Every time the update function is called the motor control reads values from all the proximity sensors (one at a time to avoid conflicts). It then makes decisions of what speed to set the motors at based on the data from the sensors.

If there is something in front if it is stops and waits for either the obstacle to move or for the user to pause the cart and move it to a working location.

If it senses the cart is at in intersection it makes a turn based on the current direction it is told that it is traveling and the direction it needs to go. Before it knows it is at an intersection it waits a few cycles to insure that it wasn't just bad data. Similarly, it waits a few cycles before exiting the turn.

It constantly tries to move in a strait line. If it is veering off to the left it will slow down the right motor and if it is veering off to the right it will speed up the right motor. Only the right motor is used to ensure a more gradual change in the cart. If it is too close or too far away from the right wall it will take more intense measures to attempt to correct itself. It will also try to prevent itself from overcorrection by keeping track of the most recent previous value. This could be greatly improved if a PID algorithm was to be used.

This could also be made more modular by removing the motors and allowing the system to handle them. It could instead return the outcome of the proximity sensors, raise specific flags, or return the offsets for the right and left motors.

### Motor

This initializes the pwm pin to an output (analog) and the direction pin to be an output.

The motor contains an encoder. It allows an outside source to set the desired encoder output and then uses a proportional algorithm to change the PWM value it sends every time it is told to update. Similarly to the motor control, this could be improved by using a PID algorithm however because it is updating at such a high frequency it is not as necessary.

### Encoder

This initializes the pins for channels A and B of the encoder to inputs.

Only responsible for returning the Pulse Width of the "encoder". The encoder actually functions as a tachometer in that it returns square waves. I chose to force it to return the sum of channel A and B for the average so that it would be a larger number and allow for more precise changes.

### Proximity Sensor Array

Initializes the proximity sensors using a class from a third party. It keeps a track of the 5 previous values from the proximity sensor. It stores all the proximity sensors in an array (as the name implies) and provides various functions to help the user read from the proximity sensors.

### Map

This module is used to store location data and enable the cart to navigate correctly from point A to point B by making correct turns at nodes. The map holds all the map nodes and currently sets up the map to a specific building layout. In order for a user to input a custom map they would either have to change the hard-coded map for their specific floor-plan and RFID ids or would have to create a UI and way to save the map.

This map is able to create a path to a destination if given a starting location and direction. It does this using a breadth first search (BFS) algorithm to find the shortest path. There have been times when the map is unable to create a path and the cause of this bug is still unknown.

The map stores the MapNodes as an array of points to pointers to allow for immediate access to each node given that the id of each node corresponds to the index in the array. This is unnecessary, however it does allow for the possibility of having multiple maps and faster time to immediately access each node instead of having to search through all the nodes to find a specific one.

### MapNode

This is a location on the map. It is connected to one to 5 map nodes with each connected node at one of the four cardinal directions or connected to the "OTHER" direction. Each node has an ID.

### PathNode

This represents a location in a path. It is used to allow the Map to build and store paths from one destination and direction to another destination. The path nodes are connected as a double linked list where each node has a pointer to both the previous and next node.

### Utility

- The Pins.h file contains a reference to all the pin numbers to make it easier to share them around the project.
- The Direction.h file contains a reference to all the possible directions including STOPPED. This is helpful to allow for all modules to use the same strongly typed directions. 