#include "th_base.h"

int attack_clock=0;
extern int defend_flag;
extern int myItem0,myItem1;

char ifAttack(){
    if(defend_flag==1) return 0;
    if(attack_clock!=0) return 0;
    return 1;
}

void attack(){
    if(attack_clock>0) attack_clock--;
    int myID=input_data.raw_data[0]>>6;
    char enemyID=1-myID;
    if(!ifAttack()) return;
    char i;
    for(i=5;i<=11;i++){
        if(myItem0==i||myItem1==i){
            useItem(i,enemyID);
            break;
        }
    }
    if((5<=i&&i<=7)||(9<=i&&i<=11)) attack_clock=20;
}
