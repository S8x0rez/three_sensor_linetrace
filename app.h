/* -*- coding: utf-8 -*- */
/**
   @file		app.h
   @brief		ライントレース サンプル (カラーセンサー1基バージョン)
*/

#ifndef APP_H_
#define APP_H_

/* Includes ---------------------------------------------------------*/
/*
 *  ターゲット依存の定義
 */
#include <target_test.h>
#include <stdlib.h>

#include "sensor.h"

/* typedef ----------------------------------------------------------*/

typedef struct font_property {
	lcdfont_t	type;
	int32_t		width;
	int32_t		height;
} FONT_PROPERTY;

/* define -----------------------------------------------------------*/
/*
 *  ターゲットに依存する可能性のある定数の定義
 */
#ifndef STACK_SIZE
#define STACK_SIZE		(4096)		/* タスクのスタックサイズ */
#endif /* STACK_SIZE */

#define SYS_STAT_HIGH				(2)
#define SYS_STAT_LOW				(1)
#define SYS_STAT_STOP				(0)

#define TURN_RIGHT          (1)	// for steer
#define TURN_LEFT           (2)	// for steer

/* macro ------------------------------------------------------------*/

/* variables --------------------------------------------------------*/

/* function prototypes ----------------------------------------------*/
#ifndef TOPPERS_MACRO_ONLY
extern void linetrace_cyclic_task (intptr_t);
extern void linetrace_cyclic_ultrasonic (intptr_t);
extern void linetrace_main_task (intptr_t);
#endif /* TOPPERS_MACRO_ONLY */

void exe_buzzer();
void wait_entry_btn_pushed();

void change_action(int, COLOR_VER *);
void adjust_stop_distance();

void pid_motor_steer();

#endif /* APP_H_ */
