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
#### Transmission
SerialOutputString: This function is the most essential function in the serial communicaion process. It is used to output a specific string over the serial port using a pre-determined baud rate. The baud rate must match that at the receiving end.
SerialInitialise: Initialises the serial port and specified baud rate
### Testing
Messages can manually be sent over the serial port using the SerialOutputBuffer. To confirm that these messages are being sent/received, a terminal such as the PuTTY terminal can be used. The PuTTY terminal should be given the correct COM line to read from, and the appropriate baud rate. When a message is written into the SerialOutputBuffer and the code is run, the PuTTY terminal should receive the string. 

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

## LDR/Gameplay Module 
### Summary
There are 4 LDRs used in the game that are setup to work with the ADC ports. There is one LDR which is used as the starting point of the game, and when this LDR is blocked for three seconds, the game begins. Hardware timers are used to time the game, one timer counts every seconds and another timer counts every 100th of a second. Once the game is being played, the fast-acting timer checks to see whether any of the other three LDRs have been blocked. If all three have been blocked once, then the game is finished.

### Usage
The LDR module features the resistive sensors required for this assignment and it they are used inside the maze. This module is essentially the module where the game is run and the game settings are configured.
The LDRs are powered off of the STM board but they are wired with resistors to ground. A wire connects the LDR output to a comparator which is compared to a tuned potentiometer. The inputs to the ADC ports are taken from the four outputs of the LM324 comparator

### Valid input
Changing light levels incident on the LDR are valid for using this module. The best way to use this is to have a very bright light in contrast with a very dark light.

### Functions and modularity
The ADC ports are used to check the readings from the LDRs and then interpreted using a scale value. This module only has a 2-value scale as it only cares if the LDR is on or off. The scale values are then stored in the ADC port which can be used in the main function. 
The timer code is taken from the previous assignment but has a struct to initialise the timers and some other functions to configure the timers
The gameplay code is a separate file and contains functions relating to how the game is played. This includes functions that call the ADC, interrupt, and timer initialisation functions from the other modular files. This part of the code has a struct called GameData which contains the game setting information and configuration, this can be changed to change the game settings slightly. It also contains a function called FlashEndLights which is called once all the LDRs/checkpoints have been passed and this flashes until the game is reset.

### Testing
To test this module, the STM LEDs have been configured to visually debug the system. The LDRs are set to trigger 1 of 2 LEDs, an LED for off and an LED for on. If the LED changes when you block the LDR then the potentiometer, LDR, and code has been tuned effectively. 
The LEDs are then also used to test whether the gameplay is working smoothly. During phase 1, the LEDs are set to only respond to the starting point LDR and will change when that LDR is blocked. Between game phase 1 and 2, three LEDs flash to inform the user that the game is about to begin. Then in the game playing phase, 2 LEDs are then set to respond to 1 LDR as describe earlier. Since there are 3 LDRs used in this phase, then 6 LEDs are used. When each LDR has been blocked once, you know the game is finished because the cool end light patterns will flash. To reset the game, hit the blue button and you will see all the LEDs flash and then the game will reset to phase 1. 


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





