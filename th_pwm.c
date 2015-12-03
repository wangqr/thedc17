/*
 * th_pwm.c
 *
 *      Author: wangqr
 */

#include "th_pwm.h"
#include <math.h>
#include <stdio.h>

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

void goto_pos(uint8_t x, uint8_t y){
    update_flag &= 0xfe;
    while(update_flag & 0x01 == 0);
    uint8_t ID = input_data.raw_data[0]>>6;
    int32_t xc, yc;
    float angle;
    float dis, true_dis;
    char buff[128];
    SetMotor(0,0);
    SetMotor(1,0);

    xc = input_data.raw_data[3+2*ID];
    yc = input_data.raw_data[4+2*ID];
    angle = diff_angle(atan2(y-yc,x-xc), current_dir);
    dis = sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));

    while(dis > Speed * 200000){
        sprintf(buff, "a=%d d=%d x=%d y=%d\n", (int)(angle/pi*180), (int)dis, (int)xc, (int)yc);
        log_m(buff);
        if(abs(angle)>0.5236){

            if(angle > 0){
                log_m(" r");
                SetMotor(0,1000);
                SetMotor(1,-1000);
                delay_us(TurningTime * angle / pi * 2);
            }
            else{
                log_m(" l");
                SetMotor(0,-1000);
                SetMotor(1,1000);
                delay_us(-TurningTime * angle / pi * 2);
            }
           angle = 0;
        }
        update_flag &= 0xfe;
        SetMotor(0,1000);
        SetMotor(1,1000);
        delay_us(70000);
        SetMotor(0,0);
        SetMotor(1,0);
        while(update_flag & 0x01 == 0);
        true_dis = sqrt((xc - input_data.raw_data[3+2*ID])*(xc - input_data.raw_data[3+2*ID])
                + (yc - input_data.raw_data[4+2*ID]) * (yc - input_data.raw_data[4+2*ID]));
        if(true_dis > 6 && true_dis < 10){
            Speed = Speed * 0.9 + 0.1 * (true_dis / 70000);
        }
        xc = input_data.raw_data[3+2*ID];
        yc = input_data.raw_data[4+2*ID];
        dis = sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));

        if(update_flag & 0x01){
            update_flag &= 0xfe;
            angle = diff_angle(atan2(y-yc,x-xc), current_dir);
        }
    }
    if(abs(angle)>0.5236){
        if(angle > 0){
            SetMotor(0,1000);
            SetMotor(1,-1000);
            last_turn_time += TurningTime * angle / pi * 2;
            delay_us(TurningTime * angle / pi * 2);
        }
        else{
            SetMotor(0,-1000);
            SetMotor(1,1000);
            last_turn_time -= TurningTime * angle / pi * 2;
            delay_us(-TurningTime * angle / pi * 2);
        }
    }
    delay_us(dis / Speed);
    SetMotor(0,0);
    SetMotor(1,0);
}

void follow_car(){
    update_flag &= 0xfe;
    while(update_flag & 0x01 == 0);
    int8_t ID = input_data.raw_data[0]>>6;
    int32_t xc, yc;
    int32_t x=255-input_data.raw_data[3+2*(1-ID)];
    int32_t y=255-input_data.raw_data[4+2*(1-ID)];
    float angle;
    float dis, true_dis;
    char buff[128];
    SetMotor(0,0);
    SetMotor(1,0);

    xc = input_data.raw_data[3+2*ID];
    yc = input_data.raw_data[4+2*ID];
    angle = diff_angle(atan2(y-yc,x-xc), current_dir);
    dis = sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));

    while(dis > Speed * 200000){
        sprintf(buff, "a=%d d=%d x=%d y=%d\n", (int)(angle/pi*180), (int)dis, (int)xc, (int)yc);
        log_m(buff);
        if(abs(angle)>0.5236){

            if(angle > 0){
                log_m(" r");
                SetMotor(0,1000);
                SetMotor(1,-1000);
                delay_us(TurningTime * angle / pi * 2);
            }
            else{
                log_m(" l");
                SetMotor(0,-1000);
                SetMotor(1,1000);
                delay_us(-TurningTime * angle / pi * 2);
            }
           angle = 0;
        }
        update_flag &= 0xfe;
        SetMotor(0,1000);
        SetMotor(1,1000);
        delay_us(70000);
        SetMotor(0,0);
        SetMotor(1,0);
        while(update_flag & 0x01 == 0);
        true_dis = sqrt((xc - input_data.raw_data[3+2*ID])*(xc - input_data.raw_data[3+2*ID])
                + (yc - input_data.raw_data[4+2*ID]) * (yc - input_data.raw_data[4+2*ID]));
        if(true_dis > 6 && true_dis < 10){
            Speed = Speed * 0.9 + 0.1 * (true_dis / 70000);
        }
        xc = input_data.raw_data[3+2*ID];
        yc = input_data.raw_data[4+2*ID];
        x=(int32_t)255 - input_data.raw_data[3+2*(1-ID)];
        y=(int32_t)255 - input_data.raw_data[4+2*(1-ID)];
        dis = sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));

        if(update_flag & 0x02){
            update_flag &= 0xfd;
            angle = diff_angle(atan2(y-yc,x-xc), current_dir);
        }
    }
    if(abs(angle)>0.5236){
        if(angle > 0){
            SetMotor(0,1000);
            SetMotor(1,-1000);
            last_turn_time += TurningTime * angle / pi * 2;
            delay_us(TurningTime * angle / pi * 2);
        }
        else{
            SetMotor(0,-1000);
            SetMotor(1,1000);
            last_turn_time -= TurningTime * angle / pi * 2;
            delay_us(-TurningTime * angle / pi * 2);
        }
    }
    delay_us(dis / Speed);
    SetMotor(0,0);
    SetMotor(1,0);
}

