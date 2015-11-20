#include "th_base.h"

extern char defend_flag;
extern int xc,yc;
int traps[100][2];

void trap(){
    int trap_count=0;
    int i;
    if(input_data.raw_data[1]&7==4){
        if((input_data.raw_data[1]>>6)==1){
            for(i=0;i<trap_count;i++){
                if(abs(traps[i][0]-xc)+abs(traps[i][1]-yc)<=10) break;
            }
            if(i==trap_count)
            {
                traps[trap_count][0]=xc;
                traps[trap_count][1]=yc;
                trap_count++;
            }
        }
    }
    else{
        if((input_data.raw_data[1]&7)==1||defend_flag) return;
        for(i=0;i<trap_count;i++){
            if(abs(traps[i][0]-xc)+abs(traps[i][1]-yc)<=15) break;
        }
        if(i<trap_count){
            if(myItem0==1||myItem1==1) useItem(0x01);
            else if(myItem0==2||myItem1==2) useItem(0x02);
        }
    }
}
