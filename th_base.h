/*
 * th_base.h
 *
 *  Created on: 2015Äê10ÔÂ28ÈÕ
 *      Author: wangqr
 */

#ifndef TH_BASE_H_
#define TH_BASE_H_

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include Files

#include "f2802x_common/include/adc.h"
#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/pwm.h"
#include "f2802x_common/include/sci.h"
#include "f2802x_common/include/wdog.h"


extern CLK_Handle myClk;
extern FLASH_Handle myFlash;
extern GPIO_Handle myGpio;
extern PIE_Handle myPie;
extern CPU_Handle myCpu;
extern PLL_Handle myPll;
extern WDOG_Handle myWDog;
extern PWM_Handle myPwm1, myPwm2;
extern SCI_Handle mySci;
extern long double TurningTime;

typedef signed char int8_t;

#include "th_pwm.h"
#include "th_sensor.h"
#include "th_thedc.h"

struct ThedcData {
    char *raw_data;
    char _raw_data[46];
};

void init_base(void);

#endif /* TH_BASE_H_ */
