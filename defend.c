#include "stdio.h"
#include "math.h"

int myID;
int myItem0,myItem1;                                 // 已有道具的ID

void initMyProps(){
    myItem0=(input_data.raw_data[2]>>4)&15;               
    myItem1=(input_data.raw_data[2])&15;
}

void useItem(int ID){                               // 给上位机发送指令装备防御道具，不会写

}

void defend(){
    initMyProps();
    if(input_data.raw_data[1]&7)==1) return;         // 如果已经装备护盾，返回
    
    int myID=input_data.raw_data[0]>>6;
    if((input_data.raw_data[19]>>myID)&1){          // 成为被攻击目标时，查看是否有护盾道具并装备
    	if(myItem0==1){
    			useItem(1);
    			return;
    		}
    		else if(myItem1==1){
    			useItem(1);
    			return;
    		}
    }

    if((input_data.raw_data[1]&7)==2||(input_data.raw_data[1]&7)==4) {             // 处于负面状态时，查看是否有防御道具并装备
    	for(int i=1;i<4;i++){
    		if(myItem0==i){
    			useItem(ID);
    			break;
    		}
    		else if(myItem1==i){
    			useItem(ID);
    			break;
    		}
    	}
    }
}
