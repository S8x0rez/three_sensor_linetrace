#ifndef _COLOR_H_
#define _COLOR_H_

// センサーのポートを設定
#define PORT_ULTRASONIC_SENSOR	(EV3_PORT_1)
#define PORT_COLOR_SENSOR_R		(EV3_PORT_2)
#define PORT_COLOR_SENSOR_C		(EV3_PORT_3)
#define PORT_COLOR_SENSOR_L		(EV3_PORT_4)

#define LINE_NONE	0b111
#define LINE_L		0b011
#define LINE_C		0b101
#define LINE_R		0b110
#define LINE_LC		0b001
#define LINE_CR		0b100
#define LINE_LR		0b010
#define LINE_ALL	0b000

// REFLECTモード用 路面の値
#define LINE_COLOR_BLACK				(10)
#define LINE_COLOR_WHITE				(50)
#define LINE_COLOR_THRESH			(LINE_COLOR_BLACK + LINE_COLOR_WHITE) / 2

#define BLACK 0
#define WHITE 1

typedef unsigned char uchar;

typedef union color_stat {
	uchar WORD;
	struct {
		uchar right		: 1;
		uchar center	: 1;
		uchar left		: 1;
	} BIT;
} COLOR_STAT;

typedef struct color_stat_ver {
	COLOR_STAT pre_ver;
	COLOR_STAT cur_ver;
} COLOR_VER;

void sensor_config();

COLOR_STAT get_all_color_stat();
uchar diff_color_stat(COLOR_STAT *, COLOR_STAT *);
int update_color_stat(COLOR_VER *);

int judge_color_stat(int);
int judge_line_stat(const COLOR_VER *);

#endif
