#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdbool.h>

typedef unsigned char uchar;

typedef struct motor_property {
	int l_power;
	int r_power;
	int steer_ratio;
} MOTOR_PROPERTY;

typedef union color_stat {
	uchar WORD;
	struct BIT {
		uchar left   : 1;
		uchar center : 1;
		uchar right  : 1;
	};
} COLOR_STAT;

typedef struct color_stat_ver {
	COLOR_STAT pre_ver;
	COLOR_STAT cur_ver;
} COLOR_VER;

#define SYS_STAT_STOP 0
#define SYS_STAT_LOW 1
#define SYS_STAT_HIGH 2

#define STOP_DISTANCE 20.4
#define SLOW_DOWN_BUF 5.0
#define STOP_BUFFERED STOP_DISTANCE + SLOW_DOWN_BUF

#define MOTOR_POWER_MAX 100
#define MOTOR_POWER_MIN -100

#define COLOR_SENSOR_L_PORT 0
#define COLOR_SENSOR_C_PORT 1
#define COLOR_SENSOR_R_PORT 2

#define COLOR_BLACK 6
#define COLOR_WHITE 91
#define COLOR_THRESH (COLOR_BLACK + COLOR_WHITE) / 2

#define LINE_NONE	0b111
#define LINE_L		0b011
#define LINE_C		0b101
#define LINE_R		0b110
#define LINE_LC		0b001
#define LINE_CR		0b100
#define LINE_LR		0b101
#define LINE_ALL	0b000

int sys_stat = 0;
double ultra_wave_value = 10000;

void exe_buzzer();
void motor_task();
void ultra_wave_task();
void wait_entry_btn_pushed();
void all_motor_stop();
void all_motor_power(int, int);
void adjust_stop_distance();

int update_color_stat(COLOR_VER *);
COLOR_STAT get_all_color_stat();
int judge_line_stat(COLOR_VER *);
int judge_color_stat(int);
int diff_color_stat(COLOR_STAT *, COLOR_STAT *);
void change_action(int);
void move_2axis();

// defined
void wait(int time);
void start_buzzer();
void stop_buzzer();
double get_ultra_wave_value();
int get_color_stat(int);
void motor_steer(int);
void set_motor_power(int);

#endif
