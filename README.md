# MTRX2700 Major Project - The a-MAZE-ing Race
## Project Details:
The a-MAZE-ing Race is a competitive maze game with the objective of reaching all the checkpoints on the maze board as fast as possible. 

## Group Members
- Sebastian O'Connell (520443826)
- Ben Ritchie (520479733)
- Luca Agius (SID)
- Will Ridley-Smith (SID)

## Roles & Responsibilities
## Will Ridley-Smith
- Joystick Module
- 3D printing

## Luca Agius
- LDR, GUI, and Serial Modules
- Documentation lead

### Sebastian O'Connell:
- LDR, GUI, and Serial Modules
- Minutes documentation

### Ben Ritchie:
- Servo Module
- Minutes documentation

## Module Breakdown
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


## GUI Timer Module 
### Summary
The GUI is generated using python code to display a timer when the game starts, and employs a leaderboard to track the quickest times. 
### Usage

### Valid input
The GUI code must be run on a valid python coding platform that has the capability to generate a GUI pop-up application (such as VSCode).
### Functions and modularity
The timer will be idle until it receives a specific packet over serial indicating the game has started - this occurs when the maze ball sits in the starting position for three seconds. The timer will continue to run until all of the checkpoints are reached. Once all the checkpoints are reached, a packet will be sent over serial indicating the game has ended. The timer will stop, reset to zero and sit idle until the game is started again. Additionally, if the total time taken to complete the game is in the top 5 quickest, it will be added to the games leaderboard. 

### Testing








