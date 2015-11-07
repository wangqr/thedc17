/*
 * th_thedc.h
 *
 *  Created on: 2015Äê10ÔÂ28ÈÕ
 *      Author: wangqr
 */

#ifndef TH_THEDC_H_
#define TH_THEDC_H_

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include Files

#include "f2802x_common/include/adc.h"
#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/sci.h"
#include "f2802x_common/include/wdog.h"

#include "th_base.h"

extern GPIO_Handle myGpio;
extern volatile struct ThedcData input_data;

__interrupt void sciaRxFifoIsr(void);

void scia_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);

#define th_thedc_send_data(x) scia_msg(x)


void init_thedc();
void UpdateData(uint8_t);

#endif /* TH_THEDC_H_ */
