#include "stdio.h"
#include "math.h"
#define limitAngle 45
#define TurningTime 500000
#define ItemEatingTime 500000
#define pi 3.1415926
#define SensorNum 6


struct Item{                  //x,y是道具位置坐标
    int x;
    int y;
}items[4];

int xl,yl,xc,yc;
bool firstTime=true;

void initItems(){
    int itemNum=0;                                                              //场上有效道具数
    if(((input_data.raw_data[1]>>3)&7)==1) itemNum=1;
    else if(((input_data.raw_data[1]>>3)&7)==2) itemNum=2;
    else if(((input_data.raw_data[1]>>3)&7)==3) itemNum=3;
    else itemNum=4;
    for(int i=0;i<itemNum;i++){
        items[i].x=input_data.raw_data[11+2*i];
        items[i].y=input_data.raw_data[12+2*i];
    }
    for(int i=itemNum;i<4;i++){
        items[i].x=-1;
        items[i].y=-1;
    }
    
    int ID=input_data.raw_data[0]>>6;                            // 选手ID
    if(firstTime){                                              // (xl,yl): 上次记录的位置坐标; (xc,yc): 本次记录的位置坐标.
        xl=input_data.raw_data[4+2*ID];
        yl=input_data.raw_data[5+2*ID];
        xc=xl;
        yc=yl;
        firstTime=false;
    }
    else{
        xl=xc;
        yl=yc;
        xc=input_data.raw_data[4+2*ID];
        yc=input_data.raw_data[5+2*ID];
    }
}

double angle(int x,int y){                                            //返回该坐标与小车连线和小车现在行驶方向的夹角（角度值）
    int carDirectionX= xc-xl, carDirectionY=yc-yl;
    int targetDirectionX= x-xc, targetDirectionY=y-yc;
    if((carDirectionX==0&&carDirectionY==0)||(targetDirectionX==0&&targetDirectionY==0) return 0;      //避免下面计算出现错误
    return acos((carDirectionX*carDirectionY+targetDirectionX*targetDirectionY)/((carDirectionX^2+carDirectionY^2)^0.5*(targetDirectionX^2+targetDirectionY^2)^0.5))/pi*180;
}

double Distance(x,y){                                                  //返回该坐标与小车距离
    return ((x-xc)^2+(y-yc)^2)^0.5;
}

int firstItem(){                                                     //返回在一定夹角范围内且离自己最近的道具
    int firstItem=-1;
    int minDistance=512;
    for(int i=0;i<4;i++){
        if(items[i].x==-1||items[i].y==-1) break;
        if(0<=(angle(items[i].x,items[i].y)&&(angle(items[i].x,items[i].y)<=limitAngle){
            if(Distance(items[i].x,items[i].y)<minDistance){
                firstItem=i;
                minDistance=Distance(items[i].x,items[i].y);
            }
        }
    }
    return firstItem;
}

void collectItems(int n){                                             
  /* 捡道具的具体实施过程。以后可以加一个判断是要捡还是要躲避这个道具的条件。
     比如自己已经有很好的道具但还没有机会使用，不希望新道具覆盖原来道具的时候，可以躲避新道具。
  */


    if(n==-1) return;                                                 //没有需要捡的道具，返回
    int j;
    for(j=0;j<SensorNum;j++){                                         //如果撞到黑线，也返回
        if(GetLightSensor(i)==0) break;
    }
    if(j<SensorNum) return;

    int carX= xc-xl, carY= yc-yl;                                          //小车方向
    int targeX= items[n].x-xc, targetY=items[n].y-yc;                      //道具与小车连线的方向

    if(abs(targetX-xc)<=nearX&&abs(targetY-yc)<=nearY){                    //如果小车已经在道具周围，停一段时间
        SetMotor(0,0);
        SetMotor(1,0);
        DELAY_US(ItemEatingTime);
        return;
    }

    if((carX==0&&carY==0)||(targetX==0&&targetY==0) return;            //避免下面计算角度时出错

    double angle1, angle2;                                            //以下一大段用来计算该向左转还是向右转T_T, 只会分类讨论T_T
    if(carY>=0){
        angle1= acos(carX/((carX^2+carY^2)^0.5))/pi*180;             
    }
    else{
        angle1= acos(carX/((carX^2+carY^2)^0.5))/pi*180+180;
    }
    if(targetY>=0){
        angle2=acos(targetX/((targetX^2+targetY^2)^0.5))/pi*180;
    }
    else{
        angle2=acos(targetX/((targetX^2+targetY^2)^0.5))/pi*180+180;
    }
    bool left;
    double angle=angle(items[n].x,items[n].y);
    if(angle1<180){
        if(angle1<angle2 && angle2<180+angle1) left=true;
        else left=false;
    }
    else{
        if(angle1-180<angle2 && angle2<angle1) left=false;
        else left=true;
    }

    if(left){                                                         //向左转
        SetMotor(1,100);
        SetMotor(0,int(100-10/9*angle);                             //瞎编了一个轮子转速和小车要转过的角度的关系，不知道应该怎么算
        DELAY_US(TurningTime);
    }
    else{                                                             //向右转
        SetMotor(0,100);
        SetMotor(1,int(100-10/9*angle);
        DELAY_US(TurningTime);
    }
}

void Items(){                                                        //main函数调用它就好啦
    UpdateData();
    initItems();                        
    collectItems(firstItem());   
	return;
}
