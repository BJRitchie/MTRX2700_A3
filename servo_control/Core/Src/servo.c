#include "servo.h"
#include "timer.h"
#include "stm32f303xc.h"


#define CLOCK_FREQ 48000000
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

struct _Servo {
	// servo variables
	TIM_TypeDef* TIMx;	// timer to be used
	uint8_t channel;    // PWM channel
	float position[3];  // positions determined by the gyro
	float velocity;  // velocity determined by the gyro
	float target; 	// target position
	float target_velocity; // target velocity
	uint32_t max_pwm; 	// the max pwm the servo will allow
	uint32_t min_pwm; 	// the min pwm the servo will take
	uint32_t max_arr; 	// the max ARR value allowed
	uint32_t min_arr; 	// the min ARR value allowed

	// Controller Variables 
	HardwareTimer* TIMx_init;   // initialisation params for timer to be used for PID controller
	uint32_t loop_time; 		// time between controller updates
	float Kp; 				// servo controller gain
	float Td; 				// controller derivative time constant
	float Ti; 				// controller integral time constant
	float error_sum; 		// integral error sum 
	float error_last; 		// the previous error  
	void (*PIDController)(); 	// the controller code block to be used for this servo 
};

Servo Servo1 = {
	.TIMx = TIM2,
	.channel = 1,
	.position = {0,0,0},
	.velocity = 0,
	.target = 0,
	.target_velocity = 0,
	.max_pwm = 2500,
	.min_pwm = 700,
	.max_arr = 80000,
	.min_arr = 3000,

	// pid variables
	.TIMx_init = &TIM4_init,
	.loop_time = 10000,
	.Kp = 10,
	.Td = 0,
	.Ti = 1000,
	.error_sum = 0,
	.PIDController = 0x00
};

Servo Servo2 = {
	.TIMx = TIM2,
	.channel = 2,
//	.position = {0,0,0},
//	.velocity = 0,
//	.target = 0,
//	.target_velocity = 0,
	.max_pwm = 2500,
	.min_pwm = 700,
//
//	.TIMx_init = &TIM4_init,
//	.loop_time = 100000,
//	.Kp = 1,
//	.Td = 0,
//	.Ti = 1000,
//	.error_sum = 0,
//	.PIDController = 0x00
};

Servo Servo3 = {
	.TIMx = TIM3,
	.channel = 1,
//	.position = {0,0,0},
//	.velocity = 0,
//	.target = 0,
//	.target_velocity = 0,
	.max_pwm = 2500,
	.min_pwm = 700,
//
//	.TIMx_init = &TIM4_init,
//	.loop_time = 100000,
//	.Kp = 1,
//	.Td = 0,
//	.Ti = 1000,
//	.error_sum = 0,
//	.PIDController = 0x00
};

Servo Servo4 = {
	.TIMx = TIM3,
	.channel = 2,
//	.position = {0,0,0},
//	.velocity = 0,
//	.target = 0,
//	.target_velocity = 0,
	.max_pwm = 2500,
	.min_pwm = 700,
//
//	.TIMx_init = &TIM4_init,
//	.loop_time = 100000,
//	.Kp = 1,
//	.Td = 0,
//	.Ti = 1000,
//	.error_sum = 0,
//	.PIDController = 0x00
};


// a function to make servo move to a point in its range of motion
// Inputs:
// - Servo*: address of the servo struct being used
// - frac: the fraction of the servos range to go to (0 to 1)
void servoGoToFractionalPosition(Servo *servo, float frac) {
	uint32_t max = servo->max_pwm;
	uint32_t min = servo->min_pwm;
	double range = max - min;

	// map to pwm range
	uint32_t pwm = min + range*frac;

	// set the pwm val
	setServoPWM(servo, pwm);
}

void setServoPWM(Servo *servo, uint32_t pwm){

	if (servo->channel == 1) {
		servo->TIMx->CCR1 = pwm;
	}
	else if (servo->channel == 2) {
		servo->TIMx->CCR2 = pwm;
	}
	else if (servo->channel == 3) {
		servo->TIMx->CCR3 = pwm;
	}
	else if (servo->channel == 4) {
		servo->TIMx->CCR4 = pwm;
	}
}

void servoGoToFractionalVelocity(Servo *servo, float frac) {
	// check fractional input 
	if (frac < 0) {
		frac = 0.; 
	}
	else if (frac > 1) {
		frac = 1.; 
	}
	
	uint32_t max_arr = servo->max_arr; 
	uint32_t min_arr = servo->min_arr; 
	float slope = (float)max_arr - (float)min_arr;

	// map to arr range
	uint32_t arr = min + slope*frac;

	// check the counter 
	if (servo->TIMx->CNT > arr) {
		TIM2->CNT = 0; // reset counter
	}
	servo->TIMx->ARR = arr; // set the arr value 
}



// generate an output string describing the servo
void servoString(Servo* servo, uint8_t* str) {
	sprintf(str, "channel: %d, target_velocity: %0.3f, velocity: %0.3f",
			servo->channel, servo->target_velocity, servo->velocity);
}

float average(float arr[], uint32_t size) {
	float sum = 0;

	for (uint32_t i=0; i < size; i++ ) {
		sum += arr[i];
	}
	return (float)(sum/size);
}

void shift_array(float arr[], uint32_t size, float new_arr[], float new_value) {
	// shift up one and insert new value 
	new_arr[0] = new_value;

    for (size_t i = 1; i < (size); i++) {
        new_arr[i] = arr[i - 1];
    }
}

void setPIDController(Servo *servo, void (*PIDControllerFunction)()) {
	servo->PIDController = PIDControllerFunction; 	// set pid controller func in servo struct 
	setCompletionFunc(servo->TIMx_init, PIDControllerFunction); // set as timer completion func 
}

void initPIDController(Servo *servo, void (*PIDControllerFunction)()) {
	enable_timer_interrupt(); // enable the interrupts 
	BSP_GYRO_Init(); // enable gyro

	// set the controller function 
	setPIDController(servo, PIDControllerFunction);

	servo->PIDController = PIDControllerFunction;
	setCompletionFunc(servo->TIMx_init, PIDControllerFunction);

	// initialise a timer to call repeated checks
	// PID is the callback function
	TimerInitialise(servo->TIMx_init, servo->PIDController);

	// set the period between checks
	setPrescaleValue(servo->TIMx_init, 47);
	setARRValue(servo->TIMx_init, servo->loop_time);
}

void setServoTarget(Servo *servo, uint32_t target){
	servo->target = target; 
}

void setTargetVelocity(Servo *servo, float target_vel){
	servo->target_velocity = target_vel;
}



float pid_velocity_controller_inner(Servo* servo) {
	// unpack PID variables
	float Kp = servo->Kp;
	float Ti = servo->Ti;
	float Td = servo->Td;
	float error_sum = servo->error_sum;
	float error_last = servo->error_last;

	float error = servo->target_velocity - servo->velocity;
	error_sum += error;

	// calc PID components
	float P = error;
	float I = (1/Ti) * error_sum;
	float D = Td * (error - error_last);

	// apply PID equation 
	float arr_frac = Kp * (P + I + D); 

	// update PID values
	servo->error_last = error;
	servo->error_sum = error_sum;

	return arr_frac;
}

void update_position_values(Servo* servo, uint32_t axis) {
	float gyro_values[3];
	float new_arr[3];

	// get the current gyro values
	BSP_GYRO_GetXYZ(&gyro_values[0]);
	float new_value = gyro_values[axis]/20000;

	// shift in the new value
	float* old_arr = servo->position;
	shift_array(old_arr, 3, new_arr, new_value);

	servo->position[0] = new_arr[0];
	servo->position[1] = new_arr[1];
	servo->position[2] = new_arr[2];

}

void update_velocity_values(Servo* servo, uint32_t axis) {
	float gyro_values[3];
	float new_position[3];
	float old_pos_avg;
	float new_pos_avg;

	// calc time difference
	// clock frequency is 48MHz
	uint32_t loop_time = servo->loop_time;
	uint32_t psc = getPrescaleValue(servo->TIMx_init);
	float freq = CLOCK_FREQ/psc;
	float delay = (float)(loop_time)/freq;

	// determine current position
	float* prev_position = servo->position;
	old_pos_avg = average(prev_position, 3);

	BSP_GYRO_GetXYZ(&gyro_values[0]);
	shift_array(prev_position, 3, new_position, gyro_values[axis]);
	new_pos_avg = average(new_position, 3);

	// calc velocity
	float velocity = (new_pos_avg - old_pos_avg)/delay;

	servo->velocity = velocity;

	servo->position[0] = new_position[0];
	servo->position[1] = new_position[1];
	servo->position[2] = new_position[2];

} 

void PIDVelocityController1() {
	// PID controller for the velocity of servo 1

	// update the current velocity
	update_velocity_values(&Servo1, X_AXIS);

	// calculate the ARR fraction
	float arr_frac = pid_velocity_controller_inner(&Servo1);
	// set the velocity 
	servoGoToFractionalVelocity(&Servo1, arr_frac); 

	if (Servo1.target_velocity < 0) {
		// pwm value to the minimum
		servoGoToFractionalPosition(&Servo1, 0);
	}
	else {
		// pwm value to the max
		servoGoToFractionalPosition(&Servo1, 1);
	}
}




