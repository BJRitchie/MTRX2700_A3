# MTRX2700 Major Project - The a-MAZE-ing Race
## Project Details:
The a-MAZE-ing Race is a competitive maze game with the objective of reaching all the checkpoints on the maze board as fast as possible. 

## Group Members
- Sebastian O'Connell (520443826)
- Ben Ritchie (520479733)
- Luca Agius (SID)
- Will Ridley-Smith (SID)

## Roles & Responsibilities
### Will Ridley-Smith
- Joystick Module
- 3D printing

### Luca Agius
- LDR, GUI, and Serial Modules
- Documentation lead

### Sebastian O'Connell
- LDR, GUI, and Serial Modules
- Presentation slides

### Ben Ritchie
- Servo Module
- Minutes documentation

## Module Breakdown
![functionality](https://github.com/BJRitchie/MTRX2700_A3/assets/160685205/dbc3259f-69b1-4a21-9a1e-9e7800186aed)
## Serial Communication Module
### Summary
Serial communication is the foundation of all the modules, allowing the modules to communicate with one another to achieve modularisation in the maze game. 
### Usage
Each of the ensuing modules employ the serial module to transmit their relevant data such that the modules work together correctly. Using an interrupt based approach, all modules can function simultaneously, allowing for the game to be function smoothly. 
### Valid input
Sorted data from other modules should be inputted into the SerialOutputString function and sent over USART1 to utilise serial communication.
### Functions and modularity


### Testing


## Joystick Module
### Summary

### Usage

### Valid input

### Functions and modularity

### Testing


## Servo Module
### Summary

### Usage

### Valid input

### Functions and modularity

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
The GUI module aims to replicate the quality features of a real arcade game. This module is essential in transforming the design from a casual design into a competitive game with various interactive and engaging features. Implementing a timer with an associated leaderboard, music and checkpoint indicators changes the games usage to being a quantifiable race.  
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






