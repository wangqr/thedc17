//#define pwm 70

#include "th_base.h"
#include "th_pwm.h"
#include "th_thedc.h"

#include <time.h>
#include <stdio.h>
#include <math.h>

void FollowLine(void);
void defend(void);
void attack(void);
unsigned char LightSensorFlag;
char itemUsed;
void LightSensorInterrupt(void){
	LightSensorFlag=1;
}

void useItem(int ID, int target_id);

int main(){
    struct test_t{
        uint8_t a : 4;
        uint8_t b : 4;
    } test;
    struct test2_t{
        uint8_t a : 4;
        uint8_t b : 4;
    } test2;
    char buff[80];

	init_base();
	log_m("init OK\n");
	update_flag = 0;
	while(update_flag & 0x01 == 0);
    uint8_t ID = input_data.raw_data[0]>>6;
    int32_t xc, yc;
    test.a=1;
    test.b=0;
    test2.a=1;
    test2.b=0;
    sprintf(buff,"test %02x %04x\n",(int)(*((uint8_t*)(&test))),(int)(*((uint16_t*)(&test2))));
    log_m(buff);

	while(1){

        xc = input_data.raw_data[3+2*ID];
        yc = input_data.raw_data[4+2*ID];

        if((update_flag & 0x02) && (xc < 48 || xc > 208 || yc < 48 || yc > 208)) {
            float t = diff_angle(atan2(128-yc,128-xc), current_dir);
            if (t < - pi / 3 && t > -pi * 2 / 3){
                current_dir = current_dir<0?(current_dir + pi):(current_dir - pi);
                log_m("u turn\n");
                last_turn_time = 2 * TurningTime;
                SetMotor(0,1000);
                SetMotor(1,-1000);
                DELAY_US(last_turn_time);
            }
        }

	    useItem(0x0e,input_data.raw_data[0]>>6);

        FollowLine();
		//Items();
		defend();
 		attack();

	}
}



//followline.c

/*void FollowLine(void){
	uint8_t ls[4];
	LightSensorFlag = 1;
	while(LightSensorFlag){
		ls[0] = GetLightSensor(0);
		ls[1] = GetLightSensor(1);
		ls[2] = GetLightSensor(2);
		ls[3] = GetLightSensor(3);
	    int ID=input_data.raw_data[0]>>6;
        int xl=input_data.raw_data[3+2*ID];
        int yl=input_data.raw_data[4+2*ID];
        if((xl>107 && xl<148 && yl>47 && yl<192)||(xl>47 && xl<192 && yl>107 && yl<148)){
            log_m("b\n");
            SetMotor(0,-970);
            SetMotor(1,-970);
            DELAY_US(100000);
        }
        else if(!ls[0] && !ls[1] && !ls[2] && !ls[3]){
			SetMotor(0,970);
			SetMotor(1,970);
			DELAY_US(50000);
			LightSensorFlag = 0;
		}
		else if(xl>90 && xl<165 && yl>90 && yl<165){
            SetMotor(0,-970);
            SetMotor(1,970);
            DELAY_US(50000);
		}
		else if(ls[1] && ls[2]){
		    log_m("b+\n");
			SetMotor(0,-970);
			SetMotor(1,-970);
			DELAY_US(100000);
		}
		else if(ls[1]){
			SetMotor(0,970);
			SetMotor(1,-970);
			DELAY_US(50000);
		}
		else if(ls[2]){
			SetMotor(0,-970);
			SetMotor(1,970);
			DELAY_US(50000);
		}
		else if(ls[0] && ls[3]){

		}
		else if(ls[0]){
			SetMotor(0,970);
			SetMotor(1,-970);
			DELAY_US(50000);
		}
		else if(ls[3]){
			SetMotor(0,-970);
			SetMotor(1,970);
			DELAY_US(5000);
		}
	}
}*/
void FollowLine(void){
    uint8_t ls[4];
    //LightSensorFlag = 1;
    //while(LightSensorFlag){
    ls[0] = GetLightSensor(0);
    ls[1] = GetLightSensor(1);
    ls[2] = GetLightSensor(2);
    ls[3] = GetLightSensor(3);

    if(ls[1] && ls[2]){
        SetMotor(0,-1000);
        SetMotor(1,-1000);
        DELAY_US(5000);
    }
    else if(!ls[0] && !ls[1] && !ls[2] && !ls[3]){
        SetMotor(0,1000);
        SetMotor(1,1000);
        DELAY_US(5000);
        //LightSensorFlag = 0;
    }
    else if(ls[1]){
        last_turn_time += 5000;
        SetMotor(0,1000);
        SetMotor(1,-1000);
        DELAY_US(5000);
    }
    else if(ls[2]){
        last_turn_time -= 5000;
        SetMotor(0,-1000);
        SetMotor(1,1000);
        DELAY_US(5000);
    }
    else if(ls[0] && ls[3]){

    }
    else if(ls[0]){
        last_turn_time += 5000;
        SetMotor(0,1000);
        SetMotor(1,-1000);
        DELAY_US(5000);
    }
    else if(ls[3]){
        last_turn_time -= 5000;
        SetMotor(0,-1000);
        SetMotor(1,1000);
        DELAY_US(5000);
    }
    //}
}

int attack_clock=0;
extern int myItem0,myItem1;
void useItem(int ID, int target_id);
char ifAttack(){
    //if(defend_flag==1) {return 0;}
    if(attack_clock!=0) {return 0;}
    //log_m("att\n");
    return 1;
}

void attack(void){
    char buff[64];

    //sprintf(buff,"9=%d a=%d I0:%d I1:%d\n",_9_axis_angle,(int)(current_dir / pi * 180),myItem0,myItem1);
    //log_m(buff);
    if(attack_clock>0 && (update_flag & 1)){
        attack_clock--;
        update_flag &= 0xfe;
    }
    int myID=input_data.raw_data[0]>>6;
    int enemyID=1-myID;
    //if(!ifAttack()) return;
    int i;

    /*if(attack_clock==19){
        if(myItem0==itemUsed||myItem1==itemUsed){
            if(itemUsed==8){
                useItem(8,myID);
                log_m("8\n");
                attack_clock=20;
                return;
            }
            else{
                useItem(itemUsed,enemyID);
                log_m("i\n");
                attack_clock=20;
                return;
            }
        }

    }*/
   // if((!((input_data.raw_data[20]>>itemUsed)&1))&&(attack_clock==19)) attack_clock=0;
    //if(((input_data.raw_data[19]>>(4+myID))&1)&&(defend_flag==0)) return;
    for(i=5;i<=11;i++){

            if(i==8){
                useItem(8, myID);
                //log_m("8\n");
                //itemUsed=i;
            }
            else{
                useItem(i, enemyID);
                //log_m("i\n");
                //itemUsed=i;
            }


     }

    //if((5<=i&&i<=7)||(9<=i&&i<=11)) attack_clock=20;

}
