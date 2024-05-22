# MTRX2700 Major Project - The a-MAZE-ing Race
## Project Details:
The a-MAZE-ing Race is a competitive maze game with the objective of reaching all the checkpoints on the maze board as fast as possible. 

## Group Members
- Sebastian O'Connell (520443826)
- Ben Ritchie (520479733)
- Luca Agius (520510715)
- Will Ridley-Smith (520466021)

## Roles & Responsibilities
### Will Ridley-Smith
- Joystick Module
- Servo/Joystick integration 
- 3D printing and assembly

### Luca Agius
- LDR, GUI, and Serial Modules
- Documentation lead

### Sebastian O'Connell
- LDR, GUI, and Serial Modules
- Presentation slides
- Minutes documentation

### Ben Ritchie
- CAD design of gimbaling system 
- Servo Module
- Servo/Joystick integration
- Minutes documentation

## Module Breakdown
![Screenshot 2024-05-21 224016](https://github.com/BJRitchie/MTRX2700_A3/assets/160685205/e022e7b6-28d3-48c4-a185-cbb22f996c5b)
## Serial Communication Module
### Summary
Serial communication facilitates the transmission of data between modules to ensure they work together correctly.
### Usage
The LDR and GUI modules rely on packets sent over serial to communicate and ultimately carry out a usuable function. Using an interrupt based approach, the modules can function simultaneously, allowing for the game to function smoothly. 
### Valid input
Sorted data can be inputted into the SerialOutputString function and sent over USART1 to the PC to utilise serial communication.
### Functions and modularity
#### SerialOutputString
The SerialOutputString function is the most essential function in the serial communicaion process. It is used to output a specific string over the serial port using a pre-determined baud rate. The baud rate must match that at the receiving end.

### Testing


## Joystick Module
### Summary
Potentiometers track the rotation of a joystick on 2 axis'. A voltage divider is used to track the values of the position of the potentiometer. This voltage signal is passed into an analog to digital converter to get the values. A range is found for the joystick and then the values are mapped so that each coordinate is a value from 0-1000.

### Usage
To use the module, simply initialise the ADC with InitialiseADC() and use get_joystick_values() to get the current coordinates of the joystick. The values of the coordinates range from 0-1000. WHen the joystick is connected, x is 0 when all the way to the left and 1000 when all the way to the right. Y is 1000 when all the way forwards and 0 when all the way back. In this assignment, a loop continuously calls get_joystick_values() so that we are always checking the position of the joystick.

### Valid input
A pointer to a valid struct joystick_coordinates should be passed into the get_joystick_values() function.
Variable voltage should be connected to PA1 and PA5 and should have a max value of 3V.

### Functions and modularity
InitialiseADC() runs all required setup for the ADC module.
get_joystick_values() simply returns the current coordinates of the joystick.
All functionality is encapsulated within these functions.

### Testing
Connect inputs for the joystick module and initialise Serial. Use Serial Output to print the values from ADC to the terminal on a computer. Check that the y value varies when moving the joystick forward and backwards, and the x value varies when moving side to side.

Print the mapped x and y values to the screen and check that at the furthest forward position, y is 1000, and the furthest backwards, y is 0. Also check that all the way to the right, x is 1000 and all the way to the left, x is 0.


## Servo Module
### Summary
The servo module is designed for simple integration, where the user can input a value from 0 to 1, where 0 is the minimum allowable angular displacement and 1 is the maximum allowable position for the servo. Each servo is described inside a struct that contains the responsible hardware timer, channel and the maximum and minimum permissible PWM values. It also contains parameters that relate to velocity control functions. 

The servo module also contains a PID controller for velocity, which uses the PWM signal period to adjust the servo velocity and the on-board gyroscope to check the actual rotational velocity. The target (requested) velocity can be adjusted at any time using the setTargetVelocity function. This value is check at a regular interval by the PID controller, which will adjust the PWM period until it reaches an acceptable error range. 

### Usage
The servo module will be linked to the joystick module. The joystick module will request a value from 0 to 1 in the x and y axes and the servos will go to this position. 

One could also configure the joystick to control the rotational velocity of the board by adjusting the target velocity values, and the velocity control module will adjust the PWM period as required. 

### Valid input
A positional value from 0 to 1 can be requested. 

### Functions and modularity
#### Positional Control 
servoGoToFractionalPosition: will go to the requested fraction of its positional range. 
setServoPWM: sets the requested PWM value. 

#### Velocity Control 
initPIDController: initialises the PID controller for the servo 
setTargetVelocity: sets the target rotational velocity of the servo.
servoGoToFractionalVelocity: make the servo go at a fraction of the allowable range. 

### Testing

## LDR Module 
### Summary

### Usage

### Valid input

### Functions and modularity

### Testing


## GUI Module 
![maze](https://github.com/BJRitchie/MTRX2700_A3/assets/160685205/1296d150-01d7-41fa-b2f5-6e0b3bebf2db)
### Summary
The GUI is generated using python code to display a timer when the game starts, and employs a leaderboard to track the quickest times. 
### Usage
The GUI module aims to replicate the quality features of a real arcade game. This module is essential in transforming the design from a casual toy into a competitive game with various interactive and engaging features. Implementing a timer with an associated leaderboard, music and checkpoint indicators changes the games usage to being a quantifiable race.  
### Valid input
The GUI code must be run on a valid python coding platform that has the capability to generate a GUI pop-up application (such as VSCode).
### Functions and modularity
The timer will be idle until it receives a specific packet over serial indicating the game has started - this occurs when the maze marble sits in the starting position for three seconds. The timer will continue to run until all of the checkpoints are reached. Once all the checkpoints are reached by the marble, a packet will be sent over serial indicating the game has ended. The timer will stop, reset to zero and sit idle until the game is started again. Additionally, once the game ends, there is a pop-up request for a username input. After inputting a username, the time taken to finish the game and username will be displayed on the leaderboard in the appropriate position, with the fastest time at the top of the leaderboard.
#### Serial
The python module "pyserial" is installed onto the device and imported into the code to access serial. The serial packet is sent from the STM32F3Discovery board and interpretted by the python code.
#### tkinter
The python module "tkinter" is installed onto the device and imported into the code to generate the GUI. Within the GUI, there is a leaderboard, timer display, and username input request. 
#### Timer 
The python module "time" is imported to run the count.
#### Soundtrack
The python module "pygame" is installed onto the device and imported into the code to play music.

### Testing
- When inputting a name, the word length cannot exceed 20 characters. If it does exceed 20 characters, a warning message is displayed which requests a shorter username.
- A manual message can be sent over serial from the STM32F3Discovery board using the SerialOutputString function instead of using the LDR module to test the functionality of the checkpoints, soundtrack and sound effects, post-game username request and leaderboard. This is useful for when the LDR module is not available for use.
- If the game length exceeds the length of the music, the music will repeat to maintain game immersion
- To ensure clarity, once a checkpoint has been reached, it cannot be triggered again during the game instance. This means the sound effect will not be re-played in the circumstance where the marble moves past the checkpoint again. 





