/*
 * th_pwm.c
 *
 *      Author: wangqr
 */

#include "th_pwm.h"

PWM_Handle _th_pwm_handles[2];

void init_pwm(void){
    myPwm1 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
    myPwm2 = PWM_init((void *)PWM_ePWM2_BASE_ADDR, sizeof(PWM_Obj));
    _th_pwm_handles[0] = myPwm1;
    _th_pwm_handles[1] = myPwm2;

    GPIO_setPullUp(myGpio, GPIO_Number_0, GPIO_PullUp_Enable);
    GPIO_setPullUp(myGpio, GPIO_Number_1, GPIO_PullUp_Enable);
    GPIO_setPullUp(myGpio, GPIO_Number_2, GPIO_PullUp_Enable);
    GPIO_setPullUp(myGpio, GPIO_Number_3, GPIO_PullUp_Enable);
    GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_EPWM1A);
    GPIO_setMode(myGpio, GPIO_Number_1, GPIO_1_Mode_EPWM1B);
    GPIO_setMode(myGpio, GPIO_Number_2, GPIO_2_Mode_EPWM2A);
    GPIO_setMode(myGpio, GPIO_Number_3, GPIO_3_Mode_EPWM2B);

    CLK_enablePwmClock(myClk, PWM_Number_1);


    PWM_setPeriod(myPwm1, 1000);    // Set timer period
    PWM_setPhase(myPwm1, 0x0000);   // Phase is 0
    PWM_setCount(myPwm1, 0x0000);   // Clear counter

    // Setup TBCLK
    PWM_setCounterMode(myPwm1, PWM_CounterMode_Up);     // Count up
    PWM_disableCounterLoad(myPwm1);                     // Disable phase loading
    PWM_setHighSpeedClkDiv(myPwm1, PWM_HspClkDiv_by_1); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm1, PWM_ClkDiv_by_1);

    PWM_setShadowMode_CmpA(myPwm1, PWM_ShadowMode_Shadow);  // Load registers every ZERO
    PWM_setShadowMode_CmpB(myPwm1, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm1, PWM_LoadMode_Zero);
    PWM_setLoadMode_CmpB(myPwm1, PWM_LoadMode_Zero);

    // Setup compare
    PWM_setCmpA(myPwm1, 500);

    // Set actions
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm1, PWM_ActionQual_Clear);    // Clear PWM1A on CAU
    PWM_setActionQual_Zero_PwmA(myPwm1, PWM_ActionQual_Set);            // Set PWM1A on Zero
    PWM_setActionQual_CntUp_CmpA_PwmB(myPwm1, PWM_ActionQual_Clear);    // Clear PWM1B on CAU
    PWM_setActionQual_Zero_PwmB(myPwm1, PWM_ActionQual_Set);            // Set PWM1B on Zero

    CLK_enablePwmClock(myClk, PWM_Number_2);

    // The setup for ePWM2 is identical to ePWM1 except
    // there is no blanking window applied.
    PWM_setPeriod(myPwm2, 1000);    // Set timer period
    PWM_setPhase(myPwm2, 0x0000);   // Phase is 0
    PWM_setCount(myPwm2, 0x0000);   // Clear counter

    // Setup TBCLK
    PWM_setCounterMode(myPwm2, PWM_CounterMode_Up);     // Count up
    PWM_disableCounterLoad(myPwm2);                     // Disable phase loading
    PWM_setHighSpeedClkDiv(myPwm2, PWM_HspClkDiv_by_1); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm2, PWM_ClkDiv_by_1);

    PWM_setShadowMode_CmpA(myPwm2, PWM_ShadowMode_Shadow);  // Load registers every ZERO
    PWM_setShadowMode_CmpB(myPwm2, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm2, PWM_LoadMode_Zero);
    PWM_setLoadMode_CmpB(myPwm2, PWM_LoadMode_Zero);

    // Setup compare
    PWM_setCmpA(myPwm2, 500);

    // Set actions
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm2, PWM_ActionQual_Clear);    // Clear PWM2A on CAU
    PWM_setActionQual_Zero_PwmA(myPwm2, PWM_ActionQual_Set);            // Set PWM2A on Zero
    PWM_setActionQual_CntUp_CmpA_PwmB(myPwm2, PWM_ActionQual_Clear);    // Clear PWM2B on CAU
    PWM_setActionQual_Zero_PwmB(myPwm2, PWM_ActionQual_Set);            // Set PWM2B on Zero

    CLK_enableTbClockSync(myClk);

    //SetMotor (0,0);
    //SetMotor (1,0);
}

void SetMotor (uint8_t index, int8_t speed){
	//if((((input_data.raw_data[0] & 0x30) >> 4) != 1) || input_data.raw_data[21] != 0x0d){
		//input_data._raw_data[0];
		//speed = 0;
	//}
	if(speed > 1000)
		speed = 1000;
	else if(speed < -1000)
		speed = -1000;
	if (speed == 1000) {
		PWM_setCmpA(_th_pwm_handles[index], speed);
		PWM_setActionQual_CntUp_CmpA_PwmA(_th_pwm_handles[index], PWM_ActionQual_Disabled);    // Clear PWM1A on CAU
		PWM_setActionQual_Zero_PwmA(_th_pwm_handles[index], PWM_ActionQual_Set);            // Set PWM1A on Zero
		PWM_setActionQual_CntUp_CmpA_PwmB(_th_pwm_handles[index], PWM_ActionQual_Disabled);    // Clear PWM1B on CAU
		PWM_setActionQual_Zero_PwmB(_th_pwm_handles[index], PWM_ActionQual_Clear);            // Set PWM1B on Zero
	}
	else if (speed > 0) {
	    PWM_setCmpA(_th_pwm_handles[index], speed);
	    PWM_setActionQual_CntUp_CmpA_PwmA(_th_pwm_handles[index], PWM_ActionQual_Clear);    // Clear PWM1A on CAU
	    PWM_setActionQual_Zero_PwmA(_th_pwm_handles[index], PWM_ActionQual_Set);            // Set PWM1A on Zero
	    PWM_setActionQual_CntUp_CmpA_PwmB(_th_pwm_handles[index], PWM_ActionQual_Disabled);    // Clear PWM1B on CAU
	    PWM_setActionQual_Zero_PwmB(_th_pwm_handles[index], PWM_ActionQual_Clear);            // Set PWM1B on Zero
	}
	else if (speed == 0){
	    PWM_setActionQual_CntUp_CmpA_PwmA(_th_pwm_handles[index], PWM_ActionQual_Disabled);    // Clear PWM1A on CAU
	    PWM_setActionQual_Zero_PwmA(_th_pwm_handles[index], PWM_ActionQual_Clear);            // Set PWM1A on Zero
	    PWM_setActionQual_CntUp_CmpA_PwmB(_th_pwm_handles[index], PWM_ActionQual_Disabled);    // Clear PWM1B on CAU
	    PWM_setActionQual_Zero_PwmB(_th_pwm_handles[index], PWM_ActionQual_Clear);            // Set PWM1B on Zero
	}
	else{
	    PWM_setCmpA(_th_pwm_handles[index], -speed);
	    PWM_setActionQual_CntUp_CmpA_PwmA(_th_pwm_handles[index], PWM_ActionQual_Disabled);    // Clear PWM1A on CAU
	    PWM_setActionQual_Zero_PwmA(_th_pwm_handles[index], PWM_ActionQual_Clear);            // Set PWM1A on Zero
	    PWM_setActionQual_CntUp_CmpA_PwmB(_th_pwm_handles[index], PWM_ActionQual_Clear);    // Clear PWM1B on CAU
	    PWM_setActionQual_Zero_PwmB(_th_pwm_handles[index], PWM_ActionQual_Set);            // Set PWM1B on Zero
	}

}

