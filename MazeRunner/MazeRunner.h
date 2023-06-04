/*
  MazeRunner.h - Library for the Maze Runner Competition.
  Created by Manav Gupta, Jun 3, 2019.
  Updated by Andrew Ramsier, June 4 2023.
  Released into the public domain.
*/
#ifndef MazeRunner_h
#define MazeRunner_h

#define FORWARD_MEDIUM  70
#define FORWARD_SLOW  45
#define FORWARD_FAST  180
#define TURN_SLOW 50
#define TURN_MEDIUM 80
#define TURN_FAST 110


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
    MazeRunner(int straight, int turning);
    void setupRobot();
    void turn(char dir);
    unsigned int straightUntilIntersection();
   	void getDirectionsAvailable(unsigned int *direction_array);
    unsigned int isEndOfMaze();
    void stop();
    void complete();
    void simplify_path(char *path, int path_length);
    void write_simple_path_to_device();
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
