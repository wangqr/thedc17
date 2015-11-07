/*
 * th_base.c
 *
 *      Author: wangqr
 */


#include "th_base.h"

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














/*
    // Initialize GPIO
    GPIO_setPullUp(myGpio, GPIO_Number_28, GPIO_PullUp_Enable);
    GPIO_setPullUp(myGpio, GPIO_Number_29, GPIO_PullUp_Disable);
    GPIO_setQualification(myGpio, GPIO_Number_28, GPIO_Qual_ASync);
    GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_SCIRXDA);
    GPIO_setMode(myGpio, GPIO_Number_29, GPIO_29_Mode_SCITXDA);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
    EALLOW;    // This is needed to write to EALLOW protected registers
//   PieVectTable.SCIRXINTA = &sciaRxFifoIsr;
    ((PIE_Obj *)myPie)->SCIRXINTA = &sciaRxFifoIsr;
//   PieVectTable.SCITXINTA = &sciaTxFifoIsr;
    //((PIE_Obj *)myPie)->SCITXINTA = &sciaTxFifoIsr;
    EDIS;   // This is needed to disable write to EALLOW protected registers

    // Register interrupt handlers in the PIE vector table
    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_9, PIE_SubGroupNumber_1,
                              (intVec_t)&sciaRxFifoIsr);
    //PIE_registerPieIntHandler(myPie, PIE_GroupNumber_9, PIE_SubGroupNumber_2,
    //                          (intVec_t)&sciaTxFifoIsr);

    scia_init();        // Init SCI-A
    scia_fifo_init();   // Init SCI-A Fifos

    // Enable interrupts required for this example
    PIE_enableInt(myPie, PIE_GroupNumber_9, PIE_InterruptSource_SCIARX);
    //PIE_enableInt(myPie, PIE_GroupNumber_9, PIE_InterruptSource_SCIATX);

    CPU_enableInt(myCpu, CPU_IntNumber_9);
    CPU_enableGlobalInts(myCpu);

    while(1);
*/


    input_data.raw_data = input_data._raw_data;





















	init_pwm();
	init_sensor();
	init_thedc();


    CPU_enableGlobalInts(myCpu);
    //DELAY_US(10000);
}
