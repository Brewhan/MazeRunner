Maze Runner Library for Arduino
========================

How to Use
---------------

To include the library, please include these imports at the top of the file. Also store this directory in Arduino/hardware/libpololu-arduino/avr/libraries

	#include <MazeRunner.h>
	#include "Arduino.h"
	#include <OrangutanMotors.h>
	#include <OrangutanLCD.h>
	#include <OrangutanBuzzer.h>
	#include <OrangutanPushbuttons.h>
	#include <Pololu3pi.h>
	#include <PololuQTRSensors.h>
	#include <OrangutanAnalog.h>
	#include <OrangutanBuzzer.h>

Functions
---------------


### void setupRobot()
Initialises the robot for maze solving. The robot will not function correctly without it. It should be called in the setup() function. The code does:

- Displays Welcome Text
- Shows battery levels while waiting for the B button to be pressed. Ensure robot is on a line by this point. 
- Calibrates the sensors
- Shows the sensor readings while waiting for B button to be pressed to start the robot.


### void turn(char direction)
Turns the robot in the direction specified. Direction must be:

- L (Left)
- R (Right)
- B (Rotate 180 degrees)
- Any other character will not turn the robot

### int straightUntilIntersection()
Will go straight until the robot detects an intersection. It returns a integer of the number of grid squares it has moved. 

### void directionsAvailable(unsigned int \*direction_array)
It accepts an array of length 3 and populates the array with the directions of travel available to the robot, in the form of [leftTurnAvailable, straightAvailable, rightTurnAvailable]

### unsigned int isEndOfMaze()
Will return 1 if it is the end of the maze, 0 otherwise

### void stop()
Stops the robot