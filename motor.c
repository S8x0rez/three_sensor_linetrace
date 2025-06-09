#include "motor.h"

#include <ev3api.h>

void motor_config()
{
	// モーターの設定
	ev3_motor_config (PORT_LEFT_MOTOR, LARGE_MOTOR);
	ev3_motor_config (PORT_RIGHT_MOTOR, LARGE_MOTOR);
}

void all_motor_power(int l_power, int r_power)
{
	ev3_motor_set_power(PORT_LEFT_MOTOR, l_power);
	ev3_motor_set_power(PORT_RIGHT_MOTOR, r_power);
}

void all_motor_stop()
{
	ev3_motor_steer(PORT_LEFT_MOTOR, PORT_RIGHT_MOTOR, 0, 0);
}

void move_rotate_2axis()
{
	ev3_motor_rotate(PORT_LEFT_MOTOR, 120, 50, false);
	ev3_motor_rotate(PORT_RIGHT_MOTOR, 120, 50, true);
}
