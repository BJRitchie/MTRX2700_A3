#ifndef SERVO_HEADER
#define SERVO_HEADER

#include <stdint.h>
#include <stdio.h>
#include "stm32f303xc.h"

struct _Servo;
typedef struct _Servo Servo;

extern Servo Servo1;
extern Servo Servo2;
extern Servo Servo3;
extern Servo Servo4;

// generate an output string describing the servo
void servoString(Servo* servo, uint8_t* str);

float average(float arr[], uint32_t size);

void shift_array(float arr[], uint32_t size, float new_arr[], float new_value);

// a function to make servo move to a point in its range of motion
// Inputs:
// - Servo*: address of the servo struct being used
// - frac: the fraction of the servos range to go to (0 to 1)
void servoGoToFractionalPosition(Servo *servo, float frac);

// make a servo go at a fraction of its maximum velocity (0 to 1)
// input:
// - Servo*: address of the servo struct being used
// - frac: the fraction of the maximum velocity the servo should move at
void servoGoToFractionalVelocity(Servo *servo, float frac);

// sets the PID controller function
// input:
// - Servo*: address of the servo struct being used
// - PIDControllerFunction: a pointer to the PID controller function
void setPIDController(Servo *servo, void (*PIDControllerFunction)());

// initialises the PID Controller for servo
// Note: ensure that the PIDController function is set in the servo struct
// Input:
// - Servo*: address of the servo struct being used
// - PIDControllerFunction: a pointer to the PID controller function
void initPIDController(Servo *servo, void (*PIDControllerFunction)());

// sets servo positional target
// input:
// - Servo*: address of the servo struct being used
// - target: the position that the servo should move to
void setServoTarget(Servo *servo, uint32_t target);

// sets servo velocity target
// input:
// - Servo*: address of the servo struct being used
// - target_vel: the velocity that the servo should at
void setTargetVelocity(Servo *servo, float target_vel);

// sets PWM going to the servo
// input:
// - Servo*: address of the servo struct being used
// - pwm: the pwm value to be sent to the servo
void setServoPWM(Servo *servo, uint32_t pwm);

// the maths of the velocity pid controller
// input:
// - Servo*: address of the servo struct being used
// Output:
// - arr: the auto-reload register suggested by the controller
float pid_velocity_controller_inner(Servo* servo);

// the maths of the pid controller
// input:
// - Servo*: address of the servo struct being used
// Output:
// - pwm: the PWM frequency to be used as determined by the controller
uint32_t pid_controller_inner(Servo* servo);

// a function to update the position current position value of a servo
// Input:
// - Servo*: pointer to the servo struct being used
// - axis: the axis that the servo moves about (X_AXIS, Y_AXIS or Z_AXIS)
void update_position_values(Servo* servo, uint32_t axis);

// a function to update the  value of a servo
// Input:
// - Servo*: pointer to the servo struct being used
// - axis: the axis that the servo moves about (X_AXIS, Y_AXIS or Z_AXIS)
void update_velocity_values(Servo* servo, uint32_t axis);

// a PID controller for the velocity of servo1
void PIDVelocityController1();

// a PID controller specifically for servo1
void PIDController1();

#endif


