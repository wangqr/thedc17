//#define pwm 70

#include "th_base.h"
#include "th_pwm.h"
#include "th_thedc.h"

void FollowLine(void);
void Items(void);
void defend(void);

unsigned char LightSensorFlag;

void LightSensorInterrupt(void){
	LightSensorFlag=1;
}

int main(){
	init_base();
	//th_sensor_set_callback(LightSensorInterrupt);

	while(1){
		//if(LightSensorFlag){
			FollowLine();
		//}
		/*else{
			SetMotor(0,1000);
			SetMotor(1,1000);
		}*/
		//Items();
		//defend();
		//scia_xmit(0x00);
		//scia_xmit(0x0d);
		//scia_xmit(0x0a);
	}
}



//followline.c

void FollowLine(void){
	uint8_t ls[4];
	LightSensorFlag = 1;
	while(LightSensorFlag){
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
			LightSensorFlag = 0;
		}
		else if(ls[1]){
			SetMotor(0,1000);
			SetMotor(1,-1000);
			DELAY_US(5000);
		}
		else if(ls[2]){
			SetMotor(0,-1000);
			SetMotor(1,1000);
			DELAY_US(5000);
		}
		else if(ls[0] && ls[3]){

		}
		else if(ls[0]){
			SetMotor(0,1000);
			SetMotor(1,-1000);
			DELAY_US(5000);
		}
		else if(ls[3]){
			SetMotor(0,-1000);
			SetMotor(1,1000);
			DELAY_US(5000);
		}
	}
}
