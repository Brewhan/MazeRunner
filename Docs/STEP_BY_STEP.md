# Step by Guide
## Not for use by participants
The purpose of this guide is to give an overview of how these challenges can be solved. Whilst challenges may not be prescriptive, this guide should give you a working bot that can complete a maze.

***Please not the line numbers might change, and so should be used as a guideline***

## Task 1: The Robot's function names are corrupted, please correct them to something human-readable.

This task is designed to cause compiler errors and provide a very obvious error. It also introduces students to being clear with naming functions by understanding what the code is trying to do. 

**Before** 
```c++
line 60: char c2VsZWN0X3R1cm4(unsigned char found_left, unsigned char found_straight, unsigned char found_right) 
```
```c++
line 79: void c2ltcGxpZnk()
```

**Solution**
```c++
line 61: char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
```
```c++
line 81: void simplify_path() 
```

&nbsp;
&nbsp;
#
## Task 2: The program has been littered with many errors, try and find them for the program to be able to start
Ultimately this is an exercise in solving compiler errors and fixing bad syntax. This can be a time consuming task and has the potential to cause issues should the solution be wrong.

### Solutions
```c++
line 40: const int go_line1[] PROGMEM = "Press B";
```
`const int` is wrong, the variable is storing text, and so the type should be `const char`.
#
```c++
line 42:  const char runreduced line1[] PROGMEM = "Press C";
```
`runreduced line1[]` is not a valid variable name as it contains a space. It can be changed to  `runreduced_line1[]`.
#
```c++
line 45: const char complete_line2[] PROGMEM = "Complete"
```
This line is missing a semicolon `;` at the end.  
#
```c++
line 136: bot.directionsAvailable(directions);
```
the function name does not exist. It should be `getDirectionsAvailable(directions)`
#
```c++
line 188: bot.directionsAvailable(directions);
```
the function name does not exist. It should be `getDirectionsAvailable(directions)`
#
```c++
line 193: bot.turn(Dir);
```
the parameter `Dir`` is incorrect, it should be lowercase `dir`.

&nbsp;
&nbsp;
#
## Task 3: The Robot only turns right. In order to solve the maze, we will need the left turning case.

This task introduces us to the first of the logical problems; it does not appear in the compiler as an error.

**Solution**
```c++
line 69: else if(found_left)
line 70:  return 'R';
```
The `return 'R'` on line 70 should be `return 'L'`.

&nbsp;
&nbsp;
#
## Task 4: The Robot starts and stops with a frustrating delay, can you improve this?

This task asks us to read the code, understand the logical process, and independantly identify a latency / tuning issue. We use `delay(3000)` as an example, but some 'neccessary' delays are present throughout the application.

**Solution**
```c++
line 145:    delay(3000);
```
The delay is 3 seconds (3000ms), which is far too long. Instead, we can do `delay(1000)` which is 1 second (1000ms).

&nbsp;
&nbsp;
#
## Task 5: The Robot's speed is incorrect. Identify why this is, and correct it. (Tip: Don't overdo it!)

This is dealing the the constants we use to set the robot's speed. e.g. `TURN_SLOW`. These values each hold integer value. This means we can do interesting things like `MazeRunner bot(100, 100);` instead of using the constants themselves. This is the first task where we can have many different solutions.

**Example Solution**
```c++
line 34: MazeRunner bot(FORWARD_MEDIUM, TURN_MEDIUM);
```
&nbsp;
&nbsp;
#
## Task 6: The Robot does not stop when the maze is complete. Identify how to detect when the maze is over and stop the Robot from moving.
In this task we are understanding what a loop does and how we can identify when we want to break out of a loop.

**Solution**
```c++
line 150:  else{
              //TODO: Don't forget to add "break;" after the delay
              bot.stop();
              OrangutanLCD::clear;
              OrangutanLCD::printFromProgramSpace(complete_line1);
              OrangutanLCD::gotoXY(0,1);
              OrangutanLCD::printFromProgramSpace(complete_line2);
              delay(1000);

            }
```
The code here is missing a `break;` after the `delay(1000);` this means that the loop will always continue even though the `else` condition was met.

&nbsp;
&nbsp;
#
## Task 7: The replay method seems to be incomplete. The replay method can't navigate the previous path. Identify why this is, and correct the behavior.
This task is tricky and may require and explanation on how indexes work. Try and visualise the issue...
```c++
// HYPOTHETICAL CODE!
while(1){
    Path[0];
    0 = 0 + 0;
}
```
This will ALWAYS get the first element of path...

```c++
// HYPOTHETICAL CODE!
index = 0;
while(1){
   Path[index];
   index = index + 1; 
}
```
This will correctly increment the index each time as we are adding +1 each time we loop around.

Looking back at the code we have, we notice that it is not incrementing:
```c++
line 200: index_of_directions = index_of_directions + 0;
```

**Solution** 
We are not incrementing the index, so when the path is recalled in a loop, the index will always be zero. To solve this challenge, we should set the code to increment by + 1. E.g.
```c++
line 200: index_of_directions = index_of_directions + 1;
```

&nbsp;
&nbsp;
#
## Task 8: The Robot favours the right. Can you Identify a better way of doing this? Perhaps re-arrange the priority in the directional choice switch.
This task's solution is not the same as task 3. Instead, it is asking you to look at the logical order of this code. 
If you look at the select turn function, you can see the order of the decision making. In order to solve some mazes, you would have to change the strategy to perhaps a left hand on the wall, which means changing the order of operations from ` R > S > L > B ` to ` L > S > R > B `.
&nbsp;
**Solution** 
```c++ 
  line 67:  if(found_left)
  line 68:    return 'L';
  line 69:  else if(found_straight)
  line 70:    return 'S';
  line 71:  else if(found_right)
  line 72:    return 'R';
  line 73:  else
  line 74:    return 'B';
```
  *Tip: Later on, you might want to change the code choose a strategy based on which button is pressed.*
&nbsp;
&nbsp;
#
## Task 9: When the robot completes the replay method, it simply stops moving. To improve the Robot's morale, we should have the robot perform a celebration. For example a 'spin' or a 'shuffle'.
This is a challenge that rather than fixing code, is asking the participant to start writing their own code.
&nbsp;
Let's start by looking at this code block:
```c++
line 195: if(!bot.isEndOfMaze()){
line 196:   bot.turn(dir); 
line 197: } else {
line 198:   bot.stop();
line 199:   display.clear();
line 200:   display.printFromProgramSpace(complete_line1);
line 201:   display.gotoXY(0,1);
line 202:   display.printFromProgramSpace(complete_line2);
line 203:   break;
line 204: }
```
On line 195, we begin the `if` statement, this particular is defining what we do when the bot is *Not* at the end of the maze. What we are interested in is the `else` statement. From here we can tell the robot what to do when it reaches the end of the maze.

The following code will tell the robot to turn left 4 times. Available directions for `bot.turn()` are `L`, `R`, and `B`.

**Example Solution**
```c++
line 195: if(!bot.isEndOfMaze()){
line 196:   bot.turn(dir); 
line 197: } else {
line 198:   bot.stop();
line 199:   display.clear();
line 200:   display.printFromProgramSpace(complete_line1);
line 201:   display.gotoXY(0,1);
line 202:   display.printFromProgramSpace(complete_line2);
line 203:   bot.turn('L');
line 204:   bot.turn('L');
line 205:   bot.turn('L');
line 206:   bot.turn('L');
line 207:   break;
line 208: }
```
&nbsp;
&nbsp;
#
## Task 10: The robot should now be fully functional and ready for shipping - but - it's far from perfect. Do what you can to optimize the code to make the Robot solve the maze and subsequently re-run the solution faster!
This task is open ended and down to the team to decide what they think makes the bot better. Some ideas to consider is to check for redundant code, e.g.
```c++
line 143:   if(dir == 'L'){
line 144:     bot.turn(dir);
line 145:     delay(3000);
line 146:   }
line 147:   else{
line 148:     bot.turn(dir);
line 149:     delay(1000);
line 150:   }
```
before in task 4, we changed this delay as it was causing a slowdown, but actually, the entire if block is unnecessary.
The corrected code can be:

```c++
line 143:     bot.turn(dir);
line 144:     delay(1000);
```
Furthermore, you may decide `delay(1000);` is waiting for far too long, you can experiment with removing or reducing the delays and seeing what happens.
&nbsp;
Another thing to consider is tuning the speed of the bot as we did in task 5. When doing the replay method, perhaps you could try and make the robot much faster, since you already know the route it is going to take.
&nbsp;
Without saying too much to the participants, remember you can program the button presses with behavior. Take a look at the following
```c++
line 129: // This function is called once.
line 130: void loop() {
line 131:
line 132:    if(buttons.isPressed(BUTTON_B)){
```
if you added an `else if(buttons.isPressed(BUTTON_C))` you could have some code that would only run if you pressed the `C` button at the start.
&nbsp;
&nbsp;
#
I Have not provided any step by step instructions for further tasks.