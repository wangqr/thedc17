#include "th_base.h"
#include <stdio.h>

int myID;
int myItem0,myItem1;
char defend_flag;
char attackItem;
void initMyItems(){
    myItem0=(input_data.raw_data[2]>>4)&0x0f;
    myItem1=(input_data.raw_data[2])&0x0f;
}

void useItem(int ID, int target_id){
    scia_xmit('s');
    uint8_t q=(input_data.raw_data[0] & 0xc0) | (target_id << 4) | (ID&0x0f);
    if(q=='"'){
        scia_xmit('\\');
        scia_xmit('"');
    }
    else{
        scia_xmit(q);
    }
    scia_xmit(0x0d);
    scia_xmit(0x0a);
    scia_xmit('"');
    /*
    if(ID!=0x0e){
        sprintf(buff,"item %x%x\n", target_id, ID);
        log_m(buff);
    }
    */
}

void defend(void){
    defend_flag=0;
    initMyItems();
    if((input_data.raw_data[1]&0x01)==1) return;
    myID=input_data.raw_data[0]>>6;
    useItem(0x01,myID);
    useItem(0x02,myID);
    useItem(0x03,myID);
    /*if((input_data.raw_data[1]&0x07)==0x02||(input_data.raw_data[1]&0x07)==0x04){
        useItem(0x03,myID);
    }
    else{
        if((input_data.raw_data[19]>>myID)&1)
            useItem(0x01,myID);
        else
            useItem(0x02,myID);
    }*/
       /* if((input_data.raw_data[19]>>myID)&1){
            attackItem=input_data.raw_data[20];
            if(myItem0==1||myItem1==1){
                 useItem(0x01, 1);
                 defend_flag=1;
                 return;
            }
            else if(myItem0==2||myItem1==2) {
                 useItem(0x02, 1);
                 defend_flag=1;
                 return;
            }
        }
        if((input_data.raw_data[1]&7)==2||(input_data.raw_data[1]&7)==4) {
                        if(myItem0==3||myItem1==3){
                              useItem(0x03, 1);
                              defend_flag=1;
                              return;
                        }
                }*/


}
