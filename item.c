#include "math.h"
#include "th_base.h"
#define limitAngle 45
#define TurningTime 500000
#define ItemEatingTime 500000
#define pi 3.1415926
#define SensorNum 4
#define nearX 20
#define nearY 20

struct Item{
    int x;
    int y;
}items[4];

int xl,yl,xc,yc;
int carDirectionX,carDirectionY;
unsigned char firstTime=true;

void initItems(){
    int i;
    int itemNum;
    if(((input_data.raw_data[1]>>3)&7)==0) itemNum=0;
    else if(((input_data.raw_data[1]>>3)&7)==1) itemNum=1;
    else if(((input_data.raw_data[1]>>3)&7)==2) itemNum=2;
    else if(((input_data.raw_data[1]>>3)&7)==3) itemNum=3;
    else itemNum=4;
    for(i=0;i<itemNum;i++){
        items[i].x=input_data.raw_data[11+2*i];
        items[i].y=input_data.raw_data[12+2*i];
    }
    for(i=itemNum;i<4;i++){
        items[i].x=-1;
        items[i].y=-1;
    }
    
    int ID=input_data.raw_data[0]>>6;
    if(firstTime){
        xl=input_data.raw_data[4+2*ID];
        yl=input_data.raw_data[5+2*ID];
        xc=xl;
        yc=yl;
        carDirectionX=0;
        carDirectionY=0;
        firstTime=0;
    }
    else{
        xl=xc;
        yl=yc;
        xc=input_data.raw_data[4+2*ID];
        yc=input_data.raw_data[5+2*ID];
        if((xc!=xl)||(yc!=yl)) {
            carDirectionX=xc-xl;
            carDirectionY=yc-yl;
        }
    }
}

double angle(int x,int y){
    int targetDirectionX= x-xc, targetDirectionY=y-yc;
    if((carDirectionX==0&&carDirectionY==0)||(targetDirectionX==0&&targetDirectionY==0)) return 0;
    return acos((carDirectionX*carDirectionY+targetDirectionX*targetDirectionY)/(sqrt(carDirectionX*carDirectionX+carDirectionY*carDirectionY))*sqrt((targetDirectionX*targetDirectionX+targetDirectionY*targetDirectionY)))/pi*180;
}

double Distance(int x,int y){
    return sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));
}

int firstItem(){
    int i;
    int FirstItem;
    int minDistance=512;
    for(i=0;i<4;i++){
        if(items[i].x==-1||items[i].y==-1) break;
        if((0<=angle(items[i].x,items[i].y))&&(angle(items[i].x,items[i].y)<=limitAngle)){
            if(Distance(items[i].x,items[i].y)<minDistance){
                FirstItem=i;
                minDistance=Distance(items[i].x,items[i].y);
            }
        }
    }
    return FirstItem;
}

void collectItems(int n){                                             
    int i;
    if(n==-1) return;
    int j;
    for(j=0;j<SensorNum;j++){
        if(GetLightSensor(i)==0) break;
    }
    if(j<SensorNum) return;

    int targetX= items[n].x-xc, targetY=items[n].y-yc;

    if(abs(targetX-xc)<=nearX&&abs(targetY-yc)<=nearY){
        SetMotor(0,0);
        SetMotor(1,0);
        DELAY_US(ItemEatingTime);
        return;
    }

    if((carDirectionX==0&&carDirectionY==0)||(targetX==0&&targetY==0)) return;
    double angle1, angle2;
    if(carY>=0){
        angle1= acos(carDirectionX/(sqrt(carDirectionX*carDirectionX+carDirectionY*carDirectionY)))/pi*180;
    }
    else{
        angle1= acos(carDirectionX/(sqrt(carDirectionX*carDirectionX+carDirectionY*carDirectionY)))/pi*180+180;
    }
    if(targetY>=0){
    	 angle2= acos(targetX/(sqrt(targetX*targetX+targetY*targetY)))/pi*180;
    }
    else{
    	 angle2= acos(targetX/(sqrt(targetX*targetX+targetY*targetY)))/pi*180+180;
    }
    unsigned char left;
    double Angle=angle(items[n].x,items[n].y);
    if(angle1<180){
        if(angle1<angle2 && angle2<180+angle1) left=true;
        else left=false;
    }
    else{
        if(angle1-180<angle2 && angle2<angle1) left=false;
        else left=true;
    }

    if(left){
        SetMotor(1,1000);
        SetMotor(0,(int)(1000-100/9*Angle));
        DELAY_US(TurningTime);
    }
    else{
        SetMotor(0,1000);
        SetMotor(1,(int)(1000-100/9*Angle));
        DELAY_US(TurningTime);
    }
}

void Items(void){
    UpdateData();
    initItems();                        
    collectItems(firstItem());   
    return;
}
