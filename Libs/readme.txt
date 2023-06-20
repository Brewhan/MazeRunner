Installation:
Install Arduino IDE
provided folders "/hardware" "/libraries" "/MazeRunnerBroken" all go within the "/Documents/Arduino" directory upon install of Arduino IDE

Deployment of Code:
Tools -> Programmer -> Atmel STK500 Development Board
Tools -> Board -> Pololu Orangutan or 3pi robot w/ ATMega328p
Tools -> Port -> COM4

Ensure Robot is on and do Sketch -> Upload using programmer and !!WAIT!! until the programmer chip stops flashing - you may hear a beep once it is complete.

Running code:
With programmer chip unplugged and device still on, oress the "B" button just behind the LCD screen and wait
1st press -> Calibration
2nd press -> Should make a noise and say "GO"
3rd press -> robot starts moving

Please do not block the motors whilst the robot is running, this can cause them damage.