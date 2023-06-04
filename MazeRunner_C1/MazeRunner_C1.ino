#include <Arduino.h>
#include <MazeRunner.h>
#include <OrangutanMotors.h>
#include <OrangutanLCD.h>
#include <OrangutanBuzzer.h>
#include <OrangutanPushbuttons.h>
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanAnalog.h>
#include <OrangutanBuzzer.h>


/*
  __  __                 ____                              
 |  \/  | __ _ _______  |  _ \ _   _ _ __  _ __   ___ _ __ 
 | |\/| |/ _` |_  / _ \ | |_) | | | | '_ \| '_ \ / _ \ '__|
 | |  | | (_| |/ /  __/ |  _ <| |_| | | | | | | |  __/ |   
 |_|  |_|\__,_/___\___| |_| \_\\__,_|_| |_|_| |_|\___|_|   

 Event created by Aisha Animashaun, Andrew Gillard, Georgia Isaac, Jamie Tizzard and Manav Gupta
 Modified for challenge event by Andrew Ramsier

*/

/*
Forward Speed parameters:
  FORWARD_SLOW
  FORWARD_MEDIUM
  FORWARD_FAST

Turn Speed parameters:
  TURN_SLOW
  TURN_MEDIUM
  TURN_FAST

*/
MazeRunner bot(FORWARD_MEDIUM, TURN_MEDIUM);

unsigned int directions[3];
char path[100] = "";
char path_length = 0; // the length of the path

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup() {
 bot.setupRobot();
}

// This function decides which way to turn during the learning phase of
// maze solving.  It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char select_turn(char found_left, char found_straight, char found_right)
{
  /* Make a decision about how to turn. The following code
  / implements a left-hand-on-the-wall strategy, where we always
  / turn as far to the left as possible.
  /
  / keep in mind the order of these statements does matter to the selection process!
  */ 
  if(found_left){
    return 'R';
  }
  //forward
  else if(found_straight) {
    return 'S';
  }
  else if(found_right) {
    return 'R';
  }
  //backs
  else {
    return 'B';
  }
}

// This function is called once.
void loop() {

    if(OrangutanPushbuttons::isPressed(PORTB5)){
      OrangutanLCD::clear();
      delay(1000);
        while(1){
            //go forward until the we discover an intersection
            bot.straightUntilIntersection();
            // depending on how we code select_turn.. we can prioritize which turn to take.
            bot.getDirectionsAvailable(directions);
            // left, straight, right
            char dir = select_turn(directions[0], directions[1], directions[2]);
            
            if(!bot.isEndOfMaze()){
                bot.turn(dir);
                delay(50);
            }
            else{ 
              bot.complete();
              break;
            }
            path[path_length] = dir;
            path_length ++;
            bot.simplify_path(path, path_length);
          }
          //Once we have a path, let's put it onto the device's memory.
          bot.write_simple_path_to_device();
        
          // Detect when the C button is pressed
          while(!button_is_pressed(BUTTON_C)) {
          }
              
          OrangutanLCD::clear();
          int path_step_index = 0;

          //Repeat the maze with the new simple path.
          while(1){
                      
            bot.straightUntilIntersection();
            bot.getDirectionsAvailable(directions);
             
            char dir = path[path_step_index];
            
            if(!bot.isEndOfMaze()){
                bot.turn(dir);
              } else {
                bot.complete();
                break;
            }
          
            // incrementing this will go to the next direction in the simplified path list. 
            path_step_index++;
          }
    }
}