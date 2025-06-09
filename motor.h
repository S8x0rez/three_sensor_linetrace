#ifndef _MOTOR_H_
#define _MOTOR_H_

// モーターのポートを設定
#define PORT_RIGHT_MOTOR		(EV3_PORT_B)
#define PORT_LEFT_MOTOR			(EV3_PORT_C)

#define MAX_MOTOR_POWER       (50)
#define MIN_MOTOR_POWER       (-50)

#define STOP_DISTANCE       (20.4)
#define SLOW_DOWN_BUF      	(5.0)


typedef struct motor_property {
	int l_power;
	int r_power;

	int l_rotate;
	int r_rotate;
} MOTOR_PROPERTY;

void motor_config();

void all_motor_power(int, int);
void all_motor_rotate();
void all_motor_stop();

void move_rotate_2axis();

int calc_steer_ratio();	// for steer
void set_steer_ratio();	// for steer

#endif
