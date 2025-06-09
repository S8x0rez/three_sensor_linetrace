#include "sensor.h"

#include <ev3api.h>

void sensor_config()
{
	// センサーの設定
	ev3_sensor_config (PORT_ULTRASONIC_SENSOR, ULTRASONIC_SENSOR);
	ev3_sensor_config (PORT_COLOR_SENSOR_C, COLOR_SENSOR);
	ev3_sensor_config (PORT_COLOR_SENSOR_R, COLOR_SENSOR);
	ev3_sensor_config (PORT_COLOR_SENSOR_L, COLOR_SENSOR);
}

int judge_line_stat(const COLOR_VER *color)
{	
	if (color->cur_ver.WORD == LINE_C) {	// 0	//
		return 1;
	}
	else if ((color->cur_ver.WORD == LINE_LC || color->cur_ver.WORD == LINE_CR) && color->pre_ver.WORD == LINE_C) {   // >=90	//
		return 2;
	}
	else if ((color->cur_ver.WORD == LINE_L || color->cur_ver.WORD == LINE_R) && (color->pre_ver.WORD == LINE_NONE || color->pre_ver.WORD == LINE_C)) {	// diagonal	//
		return 2;
	}
	else {
		return 0;
	}
	
}

int judge_color_stat(int value)
{
	return value  > LINE_COLOR_THRESH ? 1 : 0;
}

COLOR_STAT get_all_color_stat()
{
	COLOR_STAT tmp;

	tmp.BIT.left = judge_color_stat(ev3_color_sensor_get_reflect(PORT_COLOR_SENSOR_L));
	tmp.BIT.center = judge_color_stat(ev3_color_sensor_get_reflect(PORT_COLOR_SENSOR_C));
	tmp.BIT.right = judge_color_stat(ev3_color_sensor_get_reflect(PORT_COLOR_SENSOR_R));

	return tmp;
}

int update_color_stat(COLOR_VER *data)
{
	COLOR_STAT stat = get_all_color_stat();

	if (diff_color_stat(&data->cur_ver, &stat)) {
		data->pre_ver.WORD = data->cur_ver.WORD;
		data->cur_ver.WORD = stat.WORD;
		
		return 1;
	}

	return 0;
}

int update_color_stat2(COLOR_VER *data)
{
	COLOR_STAT stat = get_all_color_stat();

	if (diff_color_stat(&data->cur_ver, &stat)) {
		data->pre_ver.WORD = data->cur_ver.WORD;
		data->cur_ver.WORD = stat.WORD;
		
		return 1;
	}

	return 0;
}

uchar diff_color_stat(COLOR_STAT *cur_stat, COLOR_STAT *stat)
{
	uchar diff = cur_stat->WORD ^ stat->WORD;

	return diff;
}


void print_sensor(){
	// カラーセンサ値の取り込み
	uint8_t l_color, c_color, r_color;
	char left[16],center[16],right[16];

	l_color = ev3_color_sensor_get_reflect(PORT_COLOR_SENSOR_L);
	c_color = ev3_color_sensor_get_reflect(PORT_COLOR_SENSOR_C);
	r_color = ev3_color_sensor_get_reflect(PORT_COLOR_SENSOR_R);

	// カラーセンサーの値を文字列に変換
	snprintf(left, sizeof(left), "%3d", l_color);
	snprintf(center, sizeof(center), "%3d", c_color);
	snprintf(right, sizeof(right), "%3d", r_color);

	// デバッグ情報を表示
	ev3_lcd_draw_string("L" ,10, 40);
	ev3_lcd_draw_string(left, 10, 80);

	ev3_lcd_draw_string("C", 40, 40);
	ev3_lcd_draw_string(center, 60, 80);

	ev3_lcd_draw_string("R", 70, 40);
	ev3_lcd_draw_string(right, 100, 80);
}