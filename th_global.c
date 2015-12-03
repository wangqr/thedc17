/*
 * th_global.c
 *
 *  Created on: 2015Äê10ÔÂ29ÈÕ
 *      Author: wangqr
 */


#include "th_base.h"

CLK_Handle myClk;
FLASH_Handle myFlash;
GPIO_Handle myGpio;
PIE_Handle myPie;
CPU_Handle myCpu;
PLL_Handle myPll;
WDOG_Handle myWDog;
PWM_Handle myPwm1, myPwm2;
SCI_Handle mySci;

int16_t _9_axis_angle;
int32_t _9_axis_diff;

float TurningTime = 298507.4627;  // time = TurningTime * angle / 90
float Speed = 0.00008;  // time = distance / Speed
float current_dir = 0;  // not updated until moved
float pi;

float last_turn_time;
uint8_t last_x = 0, last_y = 0;
