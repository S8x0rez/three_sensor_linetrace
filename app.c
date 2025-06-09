/* -*- coding: utf-8 -*- */
/**
   @file		app.c
   @brief		ライントレース サンプル (カラーセンサー1基バージョン)
*/

/* Includes ------------------------------------------------------------------*/
#include <ev3api.h>
#include "app.h"

#if defined (BUILD_MODULE)
#include <module_cfg.h>
#else
#include <kernel_cfg.h>
#endif

#include "motor.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
bool_t g_run_flag = true;
int sys_stat = 0;
COLOR_VER color;
FONT_PROPERTY	l_font_property = {EV3_FONT_MEDIUM};
int steer_ratio = 0;

char left[16],center[16],right[16];

/* Private functions ---------------------------------------------------------*/
/**
   @brief		ライントレースメインタスク
   @param		[in]	exinf	使用せず
   @retval		None
*/
void linetrace_cyclic_task (intptr_t		exinf)
{
	
			print_sensor();
	if (update_color_stat(&color)) {
        int move_case = judge_line_stat(&color);
		change_action(move_case, &color);
	}
}

/**
   @brief		ライントレース距離測定タスク
   @param		[in]	exinf	使用せず
   @retval		None
*/
void
linetrace_cyclic_ultrasonic (intptr_t		exinf)
{
	int ultra_wave_value = ev3_ultrasonic_sensor_get_distance(PORT_ULTRASONIC_SENSOR);
	
	if (ultra_wave_value < STOP_DISTANCE) {
		sys_stat = SYS_STAT_STOP;
		g_run_flag = false;

		//adjust_stop_distance();
		exe_buzzer(); 
		wait_entry_btn_pushed();
		return;
	}
	else if (sys_stat == SYS_STAT_LOW && ultra_wave_value < STOP_DISTANCE + SLOW_DOWN_BUF) {
		int power = (MAX_MOTOR_POWER - 1) * (ultra_wave_value - STOP_DISTANCE) / SLOW_DOWN_BUF + 1;
		all_motor_power(power, power);
	}
	else if (ultra_wave_value < STOP_DISTANCE + SLOW_DOWN_BUF && sys_stat == SYS_STAT_HIGH) {
		sys_stat = SYS_STAT_LOW;
	}
	else {
		sys_stat = SYS_STAT_HIGH;
	}

	g_run_flag = true;
}

/**
   @brief		起動時に実行されるタスク
   @param		[in]	unused	使用禁止
   @retval		None
*/
void
linetrace_main_task (intptr_t		unused)
{
	motor_config();
	sensor_config();

	ev3_lcd_set_font(l_font_property.type);
	ev3_font_get_size (l_font_property.type, &l_font_property.width, &l_font_property.height);

	wait_entry_btn_pushed();
	exe_buzzer();

	// タスクを開始する
	ev3_sta_cyc (CYCHDR1);
	ev3_sta_cyc (USONIC);
}

void wait_entry_btn_pushed()
{

}

void exe_buzzer()
{
	
}

void change_action(int move_case, COLOR_VER *color)
{
	int power = 4;
	switch (move_case) {
		case 0:
			
			break;
		case 1:
			all_motor_power(MAX_MOTOR_POWER, MAX_MOTOR_POWER);
			break;
		case 2:
			if (color->cur_ver.BIT.left == BLACK) {
				ev3_lcd_draw_string("hidari",10,40);
				power *= -1;
			}else{
				ev3_lcd_draw_string("migi  ",10,40);
				power = 4;
			}
			all_motor_power(power, -1 * power);
			break;
	}
	
				print_sensor();
}

void adjust_stop_distance()
{
	double dist;
	int power;

	while (abs(dist = ev3_ultrasonic_sensor_get_distance(PORT_ULTRASONIC_SENSOR) - STOP_DISTANCE) > 0.4) {
		power = (int)(dist * 10);
		power = power;
		all_motor_power(power, power);
	}

	all_motor_stop();
}

void pid_motor_steer()
{
	ev3_motor_steer(PORT_LEFT_MOTOR, PORT_RIGHT_MOTOR, MAX_MOTOR_POWER, steer_ratio);
	steer_ratio = steer_ratio >> 1 * -1;
}
