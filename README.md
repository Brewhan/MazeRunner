# Maze Runner Challenge Brief

*The Maze Runner Challenge is a set of tasks designed to introduce people to Robotics and  C/C++, using the Pololu 3pi robot as a platform (as well as the accompanying programmer device). Prior knowlege of programming is reccomended but not essential. If you are new to Arduino development, documents are written for Arduino IDE, but I have found some success with (and am currently using) VSCode with the Arduino plugin. You will also need a maze, you can create a maze using a very large flat, white, surface (e.g. A2 paper) and some black electrical tape. Make sure the end of the maze is a large block of black tape, as the code will assume the end of a maze is when detecys forward, left, and right as valid directions.*  

*For inspiration, you can have a look at [this](https://www.youtube.com/watch?v=ZMQbHMgK2rw) video from Veritasium, these devices won't be as fast as modern micro-mice, but it's a start!*

#
## Hello engineers and welcome to your first day on the job here at Redacted Robotics!

Unfortunately, our last robotics engineer has 'left the firm' and has left the code in a state of disrepair after trying to automate their role through a generative AI system. The same generative AI has proven less than capable of resolving these issues, leading us to be forced to hire yourself to repair and maintain this mission critical code.

It is up to you; human, (hopefully!) to use that biological thinking cap and return this code to full working order - and then some!

Please take some time to get familiar with the code itself, located  `MazeRunnerBroken.ino`. Should you need to view the source code of the helper functions, i would advise you to look at the file `MazeRunner.cpp` which is located in `Documents\Arduino\hardware\libpololu-arduino\avr\libraries`. *It is reccomended you do not change `MazeRunner.cpp` unless you know what a header file is.*

I have included a file `RUN_GUIDE.md` which ontains instructions on how to run your code on the Robot. If you need any help with terminologies, logic, or are simply stuck, feel free to ask myself, or another biological life form for assistance. Please refrain from feeding any information to an AI, they will only confuse the situation further!

I have compiled what we believe has gone wrong with the `MazeRunnerBroken.ino` file into a set of tasks. Please complete these tasks sequentially as to avoid getting lost.

#
## Required Tasks to complete

1) The Robot's function names are corrupted, please correct them to something human readable.
2) The program has been littered with errors, try and find them for the program to be able to start.
3) The Robot only turns right. In order to solve the maze, we will need the left turning case.
4) The Robot starts and stops with a frustrating delay
5) The Robot's speed is incorrect. Identify why this is, and correct it. *(Tip: Don't overdo it!)*
6) The Robot does not stop when the maze is complete. Identify how to detect when the maze is over and stop the Robot from moving, before invoking the replay function.
7) The replay method seems to be incomplete. The replay method can't navigate the previous path. Identify why this is, and correct the behavior.
8) The Robot favours the right. Can you Identify a better way of doing this? Perhaps re-arrange the priority in the directional choice switch. 
9) When the robot completes the replay method, it simply stops moving. To improve the Robot's morale, we should have the robot perform a celebration. For example a 'spin' or a 'shuffle'.
10) The robot should now be fully functional and ready for shipping - but - it's far from perfect. Do what you can to optimize the code to make the Robot solve the maze and subsquently re-run the solution faster!

#
## Additional tasks for extra credit
*You may need to check the `Arduino.cpp` file for guidance here*


1) The robot is equipped with a sound module. Create a custom tune to play during the celebration routine.
2) Pressing the B button three times is a chore, can you try please reduce this to a single press?
2) The replay method requires you to pick up the robot and return it  to the start. Can you invent a way for the robot to return to the start on it's own?

#
&copy; Andrew Ramsier 2023
