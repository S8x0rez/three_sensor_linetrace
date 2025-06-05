#include "main.h"

int main(void)
{
	wait_entry_btn_pushed();
	exe_buzzer();

	motor_task();

	exe_buzzer();
	wait_entry_btn_pushed();

	return 0;
}

void wait_entry_btn_pushed()
{

}

void exe_buzzer()
{
	start_buzzer();
	wait(2000);
	stop_buzzer();
}

void motor_task()
{
	COLOR_VER color;
	int move_case;

	while (sys_stat) {
		if (update_color_stat(&color)) {
			move_case = judge_line_stat(&color);
			
			change_action(move_case, &color);
		}
	}

	adjust_stop_distance();
}

void change_action(int move_case, COLOR_VER *color)
{
	switch (move_case) {
		case 0:
			break;
		case 1:
			all_motor_power(100, 100);
			break;
		case 2:
			move_2axis();	// タイヤ間の中心を軸として曲がり道で回転
		case 3:
			update_color_stat(color);

			int power = 40;
			if (color->pre_ver.left == 1) power *= -1;

			all_motor_power(power, -1 * power);
			break;
	}
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
		return 3;
	}
	else {
		return 0;
	}
	
}

char judge_color_stat(int value)
{
	return value > COLOR_THRESH ? 1 : 0;
}

COLOR_STAT get_all_color_stat()
{
	COLOR_STAT tmp;

	tmp.left = judge_color_stat(get_color_stat(COLOR_SENSOR_L_PORT));
	tmp.center = judge_color_stat(get_color_stat(COLOR_SENSOR_C_PORT));
	tmp.right = judge_color_stat(get_color_stat(COLOR_SENSOR_R_PORT));

	return tmp;
}

int update_color_stat(COLOR_VER *data)
{
	COLOR_STAT stat = get_all_color_stat();

	if (diff_color_stat(&data->cur_ver, &stat)) {
		data->pre_ver = data->cur_ver;

		data->cur_ver.WORD = stat.WORD;

		return 1;
	}

	return 0;
}

char diff_color_ver(COLOR_STAT *cur_stat, COLOR_STAT *stat)
{
	char diff = cur_stat->WORD ^ stat->WORD;

	return diff;
}

void adjust_stop_distance()
{
	double dist;
	int power;

	while (abs(dist = get_ultra_wave_value() - STOP_DISTANCE) > 0.4) {
		power = (int)(dist * 10);
		all_motor_power(power, power);
	}

	all_motor_stop();
}

void all_motor_power(int l_power, int r_power)
{
	set_motor_power(l_power);
	set_motor_power(r_power);
}

void all_motor_stop()
{
	motor_steer(0);
}

void ultra_wave_task()
{
	ultra_wave_value = get_ultra_wave_value();

	if (ultra_wave_value < STOP_DISTANCE) {
		sys_stat = SYS_STAT_STOP;
	}
	else if (sys_stat == SYS_STAT_LOW) {
		int tmp_power = (MOTOR_POWER_MAX - 1) * (ultra_wave_value - STOP_DISTANCE) / SLOW_DOWN_BUF + 1;
		all_motor_power(tmp_power, tmp_power);
	}
	else if (ultra_wave_value < STOP_BUFFERED && sys_stat == SYS_STAT_HIGH) {
		sys_stat = SYS_STAT_LOW;
	}
}
