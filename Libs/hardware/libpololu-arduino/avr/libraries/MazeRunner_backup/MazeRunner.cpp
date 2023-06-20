#include "MazeRunner.h"
#include "Arduino.h"

#include "../Pololu3pi/Pololu3pi.h"
#include "../OrangutanMotors/OrangutanMotors.h"
#include "../OrangutanAnalog/OrangutanAnalog.h"
#include "../OrangutanLCD/OrangutanLCD.h"
#include "../OrangutanPushbuttons/OrangutanPushbuttons.h"
#include "../OrangutanBuzzer/OrangutanBuzzer.h"

#include <avr/pgmspace.h>

static Pololu3pi bot;
static unsigned int sensors[5];

const char welcome_line1[] PROGMEM = "MazeRunner";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char welcome_line3[] PROGMEM = "Maze";
const char welcome_line4[] PROGMEM = "solver";

const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";
const char done[] PROGMEM = ">>a32";

const char levels[] PROGMEM = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

 int straight_max_speed = 60;
 int turning_max_speed = 80;
 int delay_ms = 175;
 double unit_time = 260.0;

 int white_threshold = 100;
 int grey_threshold = 200;
 int black_threshold = 600;

MazeRunner::MazeRunner() {}



MazeRunner::MazeRunner(int straight, int turning, int delay, double unit, int white, int grey, int black) {
  straight_max_speed = straight;
  turning_max_speed = turning;
  delay_ms = delay;
  unit_time = unit;

  white_threshold = white;
  grey_threshold = grey;
  black_threshold = black;
}

void MazeRunner::setupRobot() {
  // This must be called at the beginning of 3pi code, to set up the
  // sensors.  We use a value of 2000 for the timeout, which
  // corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
  bot.init(2000);

  load_custom_characters(); 
  
  // Play welcome music and display a message
  OrangutanLCD::printFromProgramSpace(welcome_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_line2);
  OrangutanBuzzer::playFromProgramSpace(welcome);
  delay(1000);

  OrangutanLCD::clear();
  OrangutanLCD::printFromProgramSpace(welcome_line3);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_line4);
  delay(1000);

  // Display battery voltage and wait for button press
  while(!OrangutanPushbuttons::isPressed(BUTTON_B)) {
    int bat = OrangutanAnalog::readBatteryMillivolts();

    OrangutanLCD::clear();
    OrangutanLCD::print(bat);
    OrangutanLCD::print("mV");
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print("Press B");

    delay(100);
  }

  // Always wait for the button to be released so that 3pi doesn't
  // start moving until your hand is away from it.
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  delay(1000);

  // Auto-calibration: turn right and left while calibrating the
  // sensors.
  for(int counter = 0; counter < 80; counter++) {
    if(counter < 20 || counter >= 60) {
      OrangutanMotors::setSpeeds(40,-40);
    } else {
      OrangutanMotors::setSpeeds(-40,40);
    }

    // This function records a set of sensor readings and keeps
    // track of the minimum and maximum values encountered.  The
    // IR_EMITTERS_ON argument means that the IR LEDs will be
    // turned on during the reading, which is usually what you
    // want.
    bot.calibrateLineSensors(IR_EMITTERS_ON);

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  stop();

  // Display calibrated values as a bar graph.
  while(!OrangutanPushbuttons::isPressed(BUTTON_B)) {
    // Read the sensor values and get the position measurement.
    unsigned int position = bot.readLine(sensors, IR_EMITTERS_ON);

    // Display the position measurement, which will go from 0
    // (when the leftmost sensor is over the line) to 4000 (when
    // the rightmost sensor is over the line) on the 3pi, along
    // with a bar graph of the sensor readings.  This allows you
    // to make sure the robot is ready to go.
    OrangutanLCD::clear();
    print_long(position);
    OrangutanLCD::gotoXY(0, 1);
    display_readings(sensors);

    delay(100);
  }

  OrangutanPushbuttons::waitForRelease(BUTTON_B);

  OrangutanLCD::clear();

  OrangutanLCD::print("Press B");   
  // Play music and wait for it to finish before we start driving.
  OrangutanBuzzer::playFromProgramSpace(go);
  delay(1000);
  OrangutanLCD::clear();
  
  while(OrangutanBuzzer::isPlaying());
}

void MazeRunner::load_custom_characters() {
  // no offset, e.g. one bar
  OrangutanLCD::loadCustomCharacter(levels + 0, 0);
  // two bars
  OrangutanLCD::loadCustomCharacter(levels + 1, 1); 
  // etc...
  OrangutanLCD::loadCustomCharacter(levels + 2, 2); 
  OrangutanLCD::loadCustomCharacter(levels + 3, 3);
  OrangutanLCD::loadCustomCharacter(levels + 4, 4);
  OrangutanLCD::loadCustomCharacter(levels + 5, 5);
  OrangutanLCD::loadCustomCharacter(levels + 6, 6);
  OrangutanLCD::clear(); 
}

// This function displays the sensor readings using a bar graph.
void MazeRunner::display_readings(const unsigned int *calibrated_values) {
  // Initialize the array of characters that we will use for the
  // graph.  Using the space, an extra copy of the one-bar
  // character, and character 255 (a full black box), we get 10
  // characters in the array.
  const int display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};

  for(unsigned int i = 0; i < 5; i++) {
    // The variable c will have values from 0 to 9, since
    // calibrated values are in the range of 0 to 1000, and
    // 1000/101 is 9 with integer math.
    char c = display_characters[calibrated_values[i]/101];

    // Display the bar graph character.
    OrangutanLCD::print(c);
  }
}

unsigned int MazeRunner::straightUntilIntersection() {
  int last_proportional = 0;
  long integral=0;
  unsigned long startTime = millis();
  while(1) {

    // Get the position of the line.
    unsigned int position = bot.readLine(sensors,IR_EMITTERS_ON);

    // The "proportional" term should be 0 when we are on the line.
    int proportional = ((int) position) - 2000;

    // Compute the derivative (change) and integral (sum) of the
    // position.
    int derivative = proportional - last_proportional;
    integral += proportional;

    // Remember the last position.
    last_proportional = proportional;

    // Compute the difference between the two motor power settings,
    // m1 - m2.  If this is a positive number the robot will turn
    // to the left.  If it is a negative number, the robot will
    // turn to the right, and the magnitude of the number determines
    // the sharpness of the turn.
    int power_difference = proportional / 20 + integral / 10000 + derivative * 3 / 2;

    // Compute the actual motor settings.  We never set either motor
    // to a negative value.
    if(power_difference > straight_max_speed)
      power_difference = straight_max_speed;
    if(power_difference < -straight_max_speed)
      power_difference = -straight_max_speed;
    
    if(power_difference < 0) {
      OrangutanMotors::setSpeeds(straight_max_speed+power_difference,straight_max_speed);
    } else {
      OrangutanMotors::setSpeeds(straight_max_speed,straight_max_speed-power_difference);
    }
    
    // We use the inner three sensors (1, 2, and 3) for
    // determining whether there is a line straight ahead, and the
    // sensors 0 and 4 for detecting lines going to the left and
    // right.
    if(sensors[1] < white_threshold && sensors[2] < white_threshold && sensors[3] < white_threshold) {
      // There is no line visible ahead, and we didn't see any
      // intersection.  Must be a dead end.
          stop();
	  return round((millis() - startTime) / unit_time);
    } else if(sensors[0] > grey_threshold || sensors[4] > grey_threshold) {
      // Found an intersection.
      stop();
      return round((millis() - startTime) / unit_time);
    }
  }
}

void MazeRunner::turn(char dir) {
  switch(dir) {
    case 'L':
    case 'l':
      setSpeedsFor(-turning_max_speed, turning_max_speed, delay_ms);
      break;
    case 'R':
    case 'r':
      // Turn right.
      setSpeedsFor(turning_max_speed, -turning_max_speed, delay_ms);
      break;
    case 'B':
    case 'b':
      // Turn around.
      setSpeedsFor(-turning_max_speed, turning_max_speed, delay_ms * 2);
      break;
    default:
      //Play annoying sound to show error
      OrangutanBuzzer::playFrequency(6000, 250, 7);
      break;
  }
  stop();
}

void MazeRunner::setSpeedsFor(int leftMotor, int rightMotor, int delay_ms) {
  OrangutanMotors::setSpeeds(leftMotor, rightMotor);
  delay(delay_ms);
}

void MazeRunner::directionsAvailable(unsigned int *direction_array) {
  read_line(sensors,IR_EMITTERS_ON);

  // Check for left and right exits.
  direction_array[0] = sensors[0] > white_threshold;
  direction_array[2] = sensors[4] > white_threshold;

  // Drive straight a bit more - this is enough to line up our
  // wheels with the intersection.
  setSpeedsFor(turning_max_speed / 2,  turning_max_speed / 2, 200);
  stop();
  // Check for a straight exit.
  unsigned int position = read_line(sensors,IR_EMITTERS_ON);
  direction_array[1] = position >= 1000 && position <= 3000;
}

unsigned int MazeRunner::isEndOfMaze() {
  read_line(sensors,IR_EMITTERS_ON);
  return sensors[1] > black_threshold 
    && sensors[2] > black_threshold 
    && sensors[3] > black_threshold 
    && sensors[4] > black_threshold 
    && sensors[0] > black_threshold;
}

void MazeRunner::stop() {
  setSpeedsFor(0, 0, 0);
}
