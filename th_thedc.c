/*
 * th_thedc.c
 *
 *      Author: wangqr
 */

#include "th_base.h"
#include <stdio.h>
#include <math.h>

void (*_th_thedc_callback)();
struct ThedcData input_data;
volatile uint8_t update_flag = 0;

void init_thedc(){

    // Initialize GPIO
    GPIO_setPullUp(myGpio, GPIO_Number_28, GPIO_PullUp_Disable);
    GPIO_setPullUp(myGpio, GPIO_Number_29, GPIO_PullUp_Disable);
    GPIO_setQualification(myGpio, GPIO_Number_28, GPIO_Qual_ASync);
    GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_SCIRXDA);
    GPIO_setMode(myGpio, GPIO_Number_29, GPIO_29_Mode_SCITXDA);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
	EALLOW;    // This is needed to write to EALLOW protected registers
//   PieVectTable.SCIRXINTA = &sciaRxFifoIsr;
	((PIE_Obj *)myPie)->SCIRXINTA = &sciaRxFifoIsr;
	EDIS;   // This is needed to disable write to EALLOW protected registers

	// Register interrupt handlers in the PIE vector table
	PIE_registerPieIntHandler(myPie, PIE_GroupNumber_9, PIE_SubGroupNumber_1,
							  (intVec_t)&sciaRxFifoIsr);

	scia_init();
	scia_fifo_init();

    PIE_enableInt(myPie, PIE_GroupNumber_9, PIE_InterruptSource_SCIARX);
    CPU_enableInt(myCpu, CPU_IntNumber_9);
}

// Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void scia_init()
{
    CLK_enableSciaClock(myClk);

    // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    SCI_disableParity(mySci);
    SCI_setNumStopBits(mySci, SCI_NumStopBits_One);
    SCI_setCharLength(mySci, SCI_CharLength_8_Bits);

    SCI_enableTx(mySci);
    SCI_enableRx(mySci);
    SCI_enableTxInt(mySci);
    SCI_enableRxInt(mySci);

    // SCI BRR = LSPCLK/(SCI BAUDx8) - 1
    SCI_setBaudRate(mySci, SCI_BaudRate_115_2_kBaud);

    SCI_enable(mySci);

    return;
}

// Transmit a character from the SCI
void scia_xmit(int a)
{
    while(SCI_getTxFifoStatus(mySci) != SCI_FifoStatus_Empty);

    SCI_putDataBlocking(mySci, a);
}

void scia_msg(char * msg)
{
    int i = 0;
    while(msg[i])
    {
        scia_xmit(msg[i]);
        i++;
    }
}

// Initialize the SCI FIFO
void scia_fifo_init()
{

    SCI_enableFifoEnh(mySci);
    SCI_resetTxFifo(mySci);
    SCI_clearTxFifoInt(mySci);
    SCI_resetChannels(mySci);
    SCI_setTxFifoIntLevel(mySci, SCI_FifoLevel_Empty);

    SCI_resetRxFifo(mySci);
    SCI_clearRxFifoInt(mySci);
    SCI_setRxFifoIntLevel(mySci, SCI_FifoLevel_Empty);
    SCI_enableRxFifoInt(mySci);

//MAGIC
	//SciaRegs.SCIFFTX.all=0xe080;
	//SciaRegs.SCIFFRX.all=0xa43f;
	//SciaRegs.SCIFFCT.all=0x0;

	//SciaRegs.SCIFFTX.all=0xc022;
	//SciaRegs.SCIFFRX.all=0x0022;
	//SciaRegs.SCIFFCT.all=0x0;
}


void defend(void);
__interrupt void sciaRxFifoIsr(void)
{
	UpdateData(1);
    // Clear Overflow flag
    SCI_clearRxFifoOvf(mySci);

    // Clear Interrupt flag
    SCI_clearRxFifoInt(mySci);

    // Issue PIE ack
    PIE_clearInt(myPie, PIE_GroupNumber_9);

    return;
}

uint16_t _th_thedc_getchar_b(void){
	while(SCI_getRxFifoStatus(mySci) == SCI_FifoStatus_Empty);
	return SCI_getData(mySci);
}

void UpdateData(uint8_t from_int){
    char buff[32];
	if(!from_int){
		_th_thedc_getchar_b();
	}
	uint8_t i;
	_9_axis_angle = 0;
	_9_axis_angle |= _th_thedc_getchar_b() & 0xff;
	_9_axis_angle |= (_th_thedc_getchar_b() & 0xff) << 8;
	_9_axis_angle = -_9_axis_angle;

	char *read_pointer = &input_data._raw_data[23-(input_data.raw_data-input_data._raw_data)];
	for (i = 0; i < 23; i++) {
        read_pointer[i] = _th_thedc_getchar_b();
        if(i>0 && i<22 && (read_pointer[i-1] == 0x0d) && (read_pointer[i] == 0x0a)){
            return;
        }
    }
    if((read_pointer[21] != 0x0d) || (read_pointer[22] != 0x0a)){
        return;
    }
    else{
        input_data.raw_data = read_pointer;
        update_flag |= 0x01;
    }
    log_m("data OK\n");

    uint8_t ID = input_data.raw_data[0]>>6;
    int32_t xc = input_data.raw_data[3+2*ID];
    int32_t yc = input_data.raw_data[4+2*ID];
    if((xc-last_x)*(xc-last_x)+(yc-last_y)*(yc-last_y)>=64) {
        float old_dir = current_dir;
        current_dir = atan2(yc-last_y, xc-last_x);
        old_dir = last_turn_time * 90 / diff_angle(current_dir, old_dir);
        last_turn_time = 0;
        if(old_dir > 230000 && old_dir < 380000){
            TurningTime = TurningTime * 0.9 + old_dir * 0.1;
            sprintf(buff," T=%d\n",(int)TurningTime);
            log_m(buff);
        }
        update_flag |= 0x02;
        last_x = xc;
        last_y = yc;
        _9_axis_diff = _9_axis_angle - (int32_t)(current_dir / pi * 32768);
    }
    else {
        current_dir = (_9_axis_angle - _9_axis_diff) * pi / 32768;
        while (current_dir < -pi){
            current_dir += 2 * pi;
        }
        while (current_dir >= pi){
            current_dir -= 2 * pi;
        }
    }

}
