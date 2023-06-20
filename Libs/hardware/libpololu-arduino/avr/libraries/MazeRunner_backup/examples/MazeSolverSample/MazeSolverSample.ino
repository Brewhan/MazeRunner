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

MazeRunner bot;

char path[100] = "";
unsigned char path_length = 0; // the length of the path

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup() {
 bot.setupRobot();
}

// This function decides which way to turn during the learning phase of
// maze solving.  It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
  // Make a decision about how to turn.  The following code
  // implements a left-hand-on-the-wall strategy, where we always
  // turn as far to the left as possible.
  if(found_left)
  return 'L';
  else if(found_straight)
  return 'S';
  else if(found_right)
  return 'R';
  else
  return 'B';
}

// This function is called once.
void loop()
{
  // Loop until we have solved the maze.
  while(1)
  {
    // FIRST MAIN LOOP BODY
    bot.straightUntilIntersection();
    
    // Check for the ending spot.
    // If all three middle sensors are on dark black, we have
    // solved the maze.
    if (bot.isEndOfMaze()) {
      break;
    }
    
    unsigned int directions[3];
    bot.directionsAvailable(directions);

    // Intersection identification is complete.
    // If the maze has been solved, we can follow the existing
    // path.  Otherwise, we need to learn the solution.
    unsigned char dir = select_turn(directions[0], directions[1], directions[2]);
    OrangutanLCD::clear();
    OrangutanLCD::print(dir );
    delay(800);
    // Make the turn indicated by the path.
    bot.turn(dir);

    // Store the intersection in the path variable.
    path[path_length] = dir;
    path_length ++;

    // Display the path on the LCD.
    // display_path();
  }
  // Solved the maze!

  while (1) {
    bot.stop();
  }
}