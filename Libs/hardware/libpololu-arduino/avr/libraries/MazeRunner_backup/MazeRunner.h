/*
  MazeRunner.h - Library for the Maze Runner Competition.
  Created by Manav Gupta, Jun 3, 2019.
  Released into the public domain.
*/
#ifndef MazeRunner_h
#define MazeRunner_h


#include "../Pololu3pi/Pololu3pi.h"
#include "../PololuQTRSensors/PololuQTRSensors.h"
#include "../OrangutanMotors/OrangutanMotors.h"
#include "../OrangutanAnalog/OrangutanAnalog.h"
#include "../OrangutanLCD/OrangutanLCD.h"
#include "../OrangutanPushbuttons/OrangutanPushbuttons.h"
#include "../OrangutanBuzzer/OrangutanBuzzer.h"

class MazeRunner
{
  public:
    MazeRunner();
    MazeRunner(int straight, int turning, int delay, double unit, int white, int grey, int black);
    void setupRobot();
    void turn(char dir);
    unsigned int straightUntilIntersection();
   	void directionsAvailable(unsigned int *direction_array);
    unsigned int isEndOfMaze();
    void stop();
  private:
  	void load_custom_characters();
  	void display_readings(const unsigned int *calibrated_values);
    void setSpeedsFor(int leftMotor, int rightMotor, int delay_ms);
     int straight_max_speed = 60;
     int turning_max_speed = 80;
     int delay_ms = 175;
     double unit_time = 260.0;

     int white_threshold = 100;
     int grey_threshold = 200;
     int black_threshold = 600;
};

#endif
