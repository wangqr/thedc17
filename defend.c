#include "th_base.h"

char myID;
int myItem0,myItem1;
char defend_flag;
char attackItem;
void initMyItems(){
    myItem0=(input_data.raw_data[2]>>4)&0x0f;
    myItem1=(input_data.raw_data[2])&0x0f;
}

void useItem(char itemID, char targetID=myID){
    char ch=myID<<6+targetID<<4+itemID;
    scia_xmit(ch);
    scia_xmit(0x0d);
    scia_xmit(0x0a);
}

void defend(void){
    defend_flag=0;
    initMyItems();
    if((input_data.raw_data[1]&7)==1) return;
    
    myID=input_data.raw_data[0]>>6;
    if((input_data.raw_data[19]>>myID)&1){
        attackItem=input_data.raw_data[20];
        if(myItem0==1||myItem1==1){
             useItem(0x01);
             defend_flag=1;
             return;
        }
        else if(myItem0==2||myItem1==2) {
             useItem(0x02);
             defend_flag=1;
             return;
        }
    }

    if((input_data.raw_data[1]&7)==2||(input_data.raw_data[1]&7)==4) {
                if(myItem0==3||myItem1==3){
                      useItem(0x03);
                      defend_flag=1;
                      return;
                }
                else if((input_data.raw_data[1]&7)==2&&attackItem>>5){
                    //如何判断小车是不是前后左右颠倒？
                    //如果是的话，要改setmotor()？ 左右反向，速度*-1？

                }
    	}
}
