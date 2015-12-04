
#ifndef TH_PWM_H_
#define TH_PWM_H_

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/pwm.h"
#include "f2802x_common/include/wdog.h"

#include "th_base.h"
extern PWM_Handle myPwm1, myPwm2;
extern PWM_Handle _th_pwm_handles[2];
void init_pwm(void);
void SetMotor(uint8_t index, int8_t speed);
void goto_pos(uint8_t x, uint8_t y);
void follow_car();
#endif /* TH_PWM_H_ */
