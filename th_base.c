/*
 * th_base.c
 *
 *      Author: wangqr
 */


#include "th_base.h"

#include <math.h>

void init_base(void){


    // Initialize all the handles needed for this application
    myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
    myCpu = CPU_init((void *)NULL, sizeof(CPU_Obj));
    myFlash = FLASH_init((void *)FLASH_BASE_ADDR, sizeof(FLASH_Obj));
    myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));
    myPie = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));
    myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));
    mySci = SCI_init((void *)SCIA_BASE_ADDR, sizeof(SCI_Obj));

    // Perform basic system initialization
    WDOG_disable(myWDog);
    CLK_enableAdcClock(myClk);
    (*Device_cal)();
    CLK_disableAdcClock(myClk);

    //Select the internal oscillator 1 as the clock source
    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);
    //DELAY_US(10000);

    // Setup the PLL for x12 /2 which will yield 60Mhz = 10Mhz * 12 / 2
    PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2);

    // Disable the PIE and all interrupts
    PIE_disable(myPie);
    PIE_disableAllInts(myPie);
    CPU_disableGlobalInts(myCpu);
    CPU_clearIntFlags(myCpu);

    // If running from flash copy RAM only functions to RAM
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

    // Setup a debug vector table and enable the PIE
    PIE_setDebugIntVectorTable(myPie);
    PIE_enable(myPie);

    input_data.raw_data = input_data._raw_data;

	init_pwm();
	init_sensor();
	init_thedc();

    CPU_enableGlobalInts(myCpu);

    pi = acos(-1);

    _9_axis_angle = 0;
}


void delay_us(float t){
    while(t>100000){
        DELAY_US(100000);
        t -= 100000;
    }
    if(t>0) DELAY_US(t);
}

void log_m(char* s){
    scia_xmit('l');
    while(*s){
        if(*s=='"'){
            scia_xmit('\\');
            scia_xmit('"');
        }
        else{
            scia_xmit(*s);
        }
        ++s;
    }
    scia_xmit('"');
}

float diff_angle(float a, float b){
    float q = a - b;
    if (q < -pi){
        q += 2*pi;
    }
    else if (q > pi){
        q -= 2*pi;
    }
    return q;
}
