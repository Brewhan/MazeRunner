# Step by Guide
## Not for use by participants
The purpose of this guide is to give an overview of how these challenges can be solved. Whilst challenges may not be prescriptive, this guide should give you a working bot that can complete a maze.

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
line 143:    delay(3000);
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

**Solution** 
```c++
line 200: index_of_directions = index_of_directions + 0;
```
We are not incrementing the index, so when the path is recalled in a loop, the index will always be zero. To solve this challenge, we should set the code to increment by + 1. E.g.
```c++
line 200: index_of_directions = index_of_directions + 1;
```



