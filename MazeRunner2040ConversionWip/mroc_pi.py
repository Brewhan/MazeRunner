#maze-runner-old.py
#
# Place the robot on the line and press A to calibrate, then press A
# again to start it following the line.  You can also press A later
# to stop the motors.
#
# This demo shows how to use the _thread library to run a fast main
# loop on one core of the RP2040.  The other core is free to run
# slower functions like updating the display without impacting the
# speed of the main loop.

from pololu_3pi_2040_robot import robot
from pololu_3pi_2040_robot.extras import editions
import time
import gc

display = robot.Display()
motors = robot.Motors()
line_sensors = robot.LineSensors()
directions = []
encoders = robot.Encoders()
encoder_count = encoders.get_counts()

# Note: It's not safe to use Button B in a
# multi-core program.
button_a = robot.ButtonA()

	
max_speed = 600
turn_speed = 900
calibration_speed = 1000
calibration_count = 100
encoder_count_max = 220
encorder_count_forward = 120
sensor_threshold = 150


display.fill(0)
display.text("Line Follower", 0, 0)
display.text("Place on line", 0, 20)
display.text("and press A to", 0, 30)
display.text("calibrate.", 0, 40)
display.show()

while not button_a.check():
    pass



def update_display(message):
    global prev_message
    display.fill(0)
    display.text("Maze Solver", 0, 0)
    display.text(f"L:{line[0]},R:{line[4]}", 0, 10)
    display.text(f"S:{line[1]},{line[2]},{line[3]}", 0, 20)
    prev_message = message
    #ms = (t2 - t1)/1000
    #display.text(f"Main loop: {ms:.1f}ms", 0, 20)
    #display.text(f'{prev_message}', 0, 20)
    # display.text('pd = '+str(power_difference), 0, 40)

    # 64-40 = 24
    scale = 24/1000

    display.fill_rect(36, 64-int(line[0]*scale), 8, int(line[0]*scale), 1)
    display.fill_rect(48, 64-int(line[1]*scale), 8, int(line[1]*scale), 1)
    display.fill_rect(60, 64-int(line[2]*scale), 8, int(line[2]*scale), 1)
    display.fill_rect(72, 64-int(line[3]*scale), 8, int(line[3]*scale), 1)
    display.fill_rect(84, 64-int(line[4]*scale), 8, int(line[4]*scale), 1)

    display.show()


display.fill(0)
display.show()
time.sleep_ms(500)

motors.set_speeds(calibration_speed, -calibration_speed)
for i in range(calibration_count/4):
    line_sensors.calibrate()

motors.off()
time.sleep_ms(200)

motors.set_speeds(-calibration_speed, calibration_speed)
for i in range(calibration_count/2):
    line_sensors.calibrate()

motors.off()
time.sleep_ms(200)

motors.set_speeds(calibration_speed, -calibration_speed)
for i in range(calibration_count/4):
    line_sensors.calibrate()

motors.off()

t1 = 0
t2 = time.ticks_us()
p = 0
line = []
starting = False
run_motors = True
last_update_ms = 0
power_difference = 0
prev_message = None 

def straight_until_intersection() -> bool:
    display.fill(0)
    display.show()

    last_p = 0
    global p, ir, t1, t2, line, max_speed, run_motors, encoder_count
    #run_motors = True
    while run_motors:
        # save a COPY of the line sensor data in a global variable
        # to allow the other thread to read it safely.
        line = line_sensors.read_calibrated()[:]
        update_display("")
        line_sensors.start_read()
        t1 = t2
        t2 = time.ticks_us()
        integral = 0

        # postive p means robot is to left of line
        if line[1] < 700 and line[2] < 700 and line[3] < 700:
            if p < 0:
                l = 0
            else:
                l = 4000
        else:
            # estimate line position
            l = (1000*line[1] + 2000*line[2] + 3000*line[3] + 4000*line[4]) // \
                sum(line)

        p = l - 2000
        d = p - last_p
        integral += p
        last_p = p
        denominator = 10 #decreasing this increases the magnitude of the power_difference - makes turns sharper
        power_difference = (p / denominator + integral / 10000 + d * 3 / 2) 

        if(power_difference > max_speed):
            power_difference = max_speed
        if(power_difference < -max_speed):
            power_difference = -max_speed

        if(power_difference < 0): 
            motors.set_speeds(max_speed+power_difference, max_speed)
        else:
            motors.set_speeds(max_speed, max_speed-power_difference)
        
        #line = line_sensors.read_calibrated()[:]
        
        
        if is_maze_end():
            update_display("End")
            end()
            break

        elif (int(line[1]) < 100) and (int(line[2]) < 100) and (int(line[3]) < 100):
            #dead end
            motors.set_speeds(0,0)

            turn("B")
            update_display("")


        elif (int(line[0]) > sensor_threshold) or (int(line[4]) > sensor_threshold):
            motors.set_speeds(0,0)
            what_turn()
            update_display("")


       


def what_turn():
    directions = get_available_directions()
    
    display.fill(0)
    dir = select_turn(directions[0], directions[1], directions[2])
    turn(dir)


def turn(dir: str):
    initial_count = encoders.get_counts()

    if dir == "L":
        motors.set_speeds(-turn_speed, turn_speed)
        # Wait until the robot has turned approximately 90 degrees left
        while not (encoders.get_counts()[0] <= initial_count[0] - int(encoder_count_max) and encoders.get_counts()[1] >= initial_count[1] +int(encoder_count_max)):
            pass
        motors.set_speeds(0, 0)

    elif dir == "R":
        motors.set_speeds(turn_speed, -turn_speed)
        # Wait until the robot has turned approximately 90 degrees right
        while not (encoders.get_counts()[0] >= initial_count[0] + int(encoder_count_max) and encoders.get_counts()[1] <= initial_count[1] - int(encoder_count_max)):
            pass
        motors.set_speeds(0, 0)

    elif dir == "S":
        pass
        # Straight - no specific encoder count adjustment needed here


    elif dir == "B":

        motors.set_speeds(-turn_speed, turn_speed)
        # Wait until the robot has turned approximately 180 degrees
        while not ((encoders.get_counts()[0] <= initial_count[0] - (int(encoder_count_max)*2)) and (encoders.get_counts()[1] >= initial_count[1] + (int(encoder_count_max)*2))):
            pass
        motors.set_speeds(0, 0)

    else:
        turn("L")
        turn("L")

    time.sleep_ms(1000)


def select_turn(found_left: bool, found_right: bool, found_straight: bool):
    if found_left:
        return "L"
    elif found_straight:
        return "S"
    elif found_right:
        return "R"
    else:
        return "B"

def is_maze_end():
    line = line_sensors.read_calibrated()[:]
    return ((int(line[0]) > 300) and (int(line[1]) > 600) and (int(line[2]) > 600) and (int(line[3]) > 600) and (int(line[4]) > 300))

def end():
    motors.off()
    global run_motors
    run_motors = False


def get_available_directions():
    display.fill(0)
    initial_count = encoders.get_counts()

    try:
        line = line_sensors.read_calibrated()[:]
    except Exception as e:
        return [False, False, False]

    left_dir = False
    right_dir = False
    straight_dir = False

    if int(line[0]) > sensor_threshold:
        left_dir = True
        encoders
                  
    if int(line[4]) > sensor_threshold: 
        right_dir = True
        

    #line up with intersection to check for straight line

    motors.set_speeds(500,500)
    while not (encoders.get_counts()[0] >= initial_count[0] + int(encorder_count_forward) and encoders.get_counts()[1] >= initial_count[1] + int(encorder_count_forward)):
        pass
    motors.set_speeds(0, 0)

    line = line_sensors.read_calibrated()[:]

    ## This could cause issues, we are trying to evaluate if the line ahead is straight...ish 
    if (int(line[1]) > sensor_threshold) and (int(line[2]) > sensor_threshold):
        straight_dir = True
    elif (int(line[2]) > sensor_threshold) and (int(line[3]) > sensor_threshold):
        straight_dir = True

    directions = [left_dir, right_dir, straight_dir]

    gc.collect()
    return directions


#This really slows things down, and usually gets a lot wrong!
def log_to_file(message, filename="logfile.txt"):
    kb_free = gc.mem_free() / 1024
    kb_used = gc.mem_alloc() / 1024
    kb_total = kb_free + kb_used
    with open(filename, 'a') as f:
        f.write(message + " " + "RAM: "+str(kb_used)+" / "+str(kb_total)+ "\n")
    gc.collect()



while run_motors:
    encoder_count = encoders.get_counts()
    #c[0] is left, c[1] is right

    straight_until_intersection()
