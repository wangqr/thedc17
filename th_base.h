
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
extern float TurningTime, Speed, current_dir, last_turn_time;
extern uint8_t last_x;
extern uint8_t last_y;
extern int16_t _9_axis_angle;
extern int32_t _9_axis_diff;
extern float pi;

typedef signed char int8_t;

#include "th_pwm.h"
#include "th_sensor.h"
#include "th_thedc.h"

#define abs(x) ((x)<0?(-(x)):(x))

struct ThedcData {
    char *raw_data;
    char _raw_data[46];
};

void init_base(void);

void delay_us(float t);

void log_m(char*);

float diff_angle(float, float);

#endif /* TH_BASE_H_ */
