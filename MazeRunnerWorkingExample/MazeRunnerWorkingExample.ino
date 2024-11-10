#include "Arduino.h"
#include <MazeRunner.h>
#include <OrangutanMotors.h>
#include <OrangutanLCD.h>
#include <OrangutanBuzzer.h>
#include <OrangutanPushbuttons.h>
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanAnalog.h>
#include <OrangutanBuzzer.h>
#include <EEPROM.h>

/*
  __  __                 ____                              
 |  \/  | __ _ _______  |  _ \ _   _ _ __  _ __   ___ _ __ 
 | |\/| |/ _` |_  / _ \ | |_) | | | | '_ \| '_ \ / _ \ '__|
 | |  | | (_| |/ /  __/ |  _ <| |_| | | | | | | |  __/ |   
 |_|  |_|\__,_/___\___| |_| \_\\__,_|_| |_|_| |_|\___|_|   

 Original Event created by Aisha Animashaun, Andrew Gillard, Georgia Isaac, Jamie Tizzard and Manav Gupta
 Adapted for challenge event by Andrew Ramsier

*/
/*
MazeRunner bot parameters:
  straight line speed,
  turning speed,
  delay between actions
*/

OrangutanLCD display;
OrangutanPushbuttons buttons;

MazeRunner bot(FORWARD_MEDIUM, TURN_MEDIUM);

unsigned int directions[3];
char path[100] = "";
unsigned char path_length = 0; // the length of the path

const char go_line1[] PROGMEM = "Press B";
const char go_line2[] PROGMEM = "to go";
const char runreduced_line1[] PROGMEM = "Press C";
const char runreduced_line2[] PROGMEM = "toReduce";
const char complete_line1[] PROGMEM = "Maze";
const char complete_line2[] PROGMEM = "Complete";

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup() {
 bot.setupRobot();
 display.printFromProgramSpace(go_line1);
 display.gotoXY(0,1);
 display.printFromProgramSpace(go_line2);
}

// This function decides which way to turn during the learning phase of
// maze solving.  It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
  // Make a decision about how to turn.  The following code
  // implements a right-hand-on-the-wall strategy, where we always
  // turn as far to the right as possible.
  
  if(found_left)
  return 'L';
  else if(found_straight)
  return 'S';
  else if(found_right)
  return 'R';
  else
  return 'B';
}

// Path simplification.  The strategy is that whenever we encounter a
// sequence xBx, we can simplify it by cutting out the dead end.  For
// example, LBL -> S, because a single S bypasses the dead end
// represented by LBL.
void simplify_path()
{
  // only simplify the path if the second-to-last turn was a 'B'
  if(path_length < 3 || path[path_length-2] != 'B')
  return;

  int total_angle = 0;
  int i;
  for(i=1;i<=3;i++)
  {
    switch(path[path_length-i])
    {
      case 'R':
      total_angle += 90;
      break;
      case 'L':
      total_angle += 270;
      break;
      case 'B':
      total_angle += 180;
      break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  total_angle = total_angle % 360;

  // Replace all of those turns with a single one.
  switch(total_angle)
  {
    case 0:
    path[path_length - 3] = 'S';
    break;
    case 90:
    path[path_length - 3] = 'R';
    break;
    case 180:
    path[path_length - 3] = 'B';
    break;
    case 270:
    path[path_length - 3] = 'L';
    break;
  }

  // The path is now two steps shorter.
  path_length -= 2;
}

// This function is called once.
void loop() {

    if(buttons.isPressed(BUTTON_B)){
      display.clear();
      delay(1000);
      
        while(1){
            bot.straightUntilIntersection();
            bot.getDirectionsAvailable(directions);
        
            unsigned char dir = select_turn(directions[0], directions[1], directions[2]);

            if(!bot.isEndOfMaze()){
                bot.turn(dir);
                delay(100);
            }
            else{
              //we want to break out of the loop if we are at the end of the maze! but something's missing....
              bot.stop();  
              display.clear;
              display.printFromProgramSpace(complete_line1);
              display.gotoXY(0,1);
              display.printFromProgramSpace(complete_line2);
              delay(5000);
              break;
            }
        
            path[path_length] = dir;
            path_length ++;
        
            simplify_path();
          }
          
          int index_of_directions = 0;
          int address = 0;
        
          for (int i=0; i < path_length; i++){
            EEPROM.write(address, path[i]);
            address++;
          }
          
          display.clear();
          display.printFromProgramSpace(runreduced_line1);
          display.gotoXY(0,1);
          display.printFromProgramSpace(runreduced_line2);
          
          while(!button_is_pressed(BUTTON_C)) {
          }
              
          delay(1000);
          display.clear();
            
          while(1){
                      
            bot.straightUntilIntersection();
            bot.getDirectionsAvailable(directions);
             
            unsigned char dir = path[index_of_directions];

            
            if(!bot.isEndOfMaze()){
                bot.turn(dir);
              } else {
                bot.stop();
                              display.clear;
              display.printFromProgramSpace(complete_line1);
              display.gotoXY(0,1);
              display.printFromProgramSpace(complete_line2);
                break;
            }
          index_of_directions = index_of_directions + 1;
          }
    }
}
