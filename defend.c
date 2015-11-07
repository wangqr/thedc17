#include "stdio.h"
#include "math.h"
#include "th_base.h"

int myID;
int myItem0,myItem1;

void initMyProps(){
    myItem0=(input_data.raw_data[2]>>4)&0x0f;
    myItem1=(input_data.raw_data[2])&0x0f;
}

void useItem(int ID){
	uint8_t i;
	//
		//scia_xmit((input_data.raw_data[0] & 0xc0) | (target_id << 4) | ID );
	    SetMotor(0,0);
		SetMotor(1,0);
	//for(i=0;i<5;i++){
		scia_xmit(0x01);
		scia_xmit(0x0d);
		scia_xmit(0x0a);
		DELAY_US(100000);
        SetMotor(0,1000);
        SetMotor(1,1000);
	//}
}

void defend(void){

	//if((input_data.raw_data[2]>>4)>=0) useItem(input_data.raw_data[2]>>4);
	//if((input_data.raw_data[2]&0x0f)>=0) useItem(input_data.raw_data[2]&0x0f);
/*
	if(input_data.raw_data[21] != 0x0d){
		return;
	}
    initMyProps();
    if((input_data.raw_data[1]&7)==1) return;
    int myID=input_data.raw_data[0]>>6;
    */
    if(/*((input_data.raw_data[19]>>myID)&1) &&*/ (input_data.raw_data[20])){
    	//if(myItem0==1){
			useItem(1);
			//return;
		//}
		//else if(myItem1==1){
			//useItem(1);
			//return;
		//}
    }
/*
    if(input_data.raw_data[1]&0x06) {
    	//for(i=1;i<4;i++){
		if(myItem0 == 3){
			useItem(3);
		}
		else if(myItem1 == 3){
			useItem(3);
		}
    	//}
    }
    */
}
