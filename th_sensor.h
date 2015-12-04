
#ifndef TH_SENSOR_H_
#define TH_SENSOR_H_

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include Files

#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/wdog.h"

#include "th_base.h"

extern GPIO_Handle myGpio;

__interrupt void xint1_isr(void);

extern void(*_th_sensor_callback)();

#define _th_light_sensor_cnt 4
extern const GPIO_Number_e _th_light_sensor_pin[4];

void init_sensor();
void th_sensor_set_callback(void(*f)());
uint8_t GetLightSensor(uint8_t index);
__interrupt void xint1_isr(void);

#endif /* TH_SENSOR_H_ */
