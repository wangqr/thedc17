#include "math.h"
#include "th_base.h"
#define limitAngle 90
#define ItemEatingTime 0
//500000
#define pi 3.1415926
#define SensorNum 4
#define nearX 20
#define nearY 20

#define abs(x) ((x)<0?(-(x)):(x))

struct Item{
    int x;
    int y;
}items[4];

int xl,yl,xc,yc;
int carDirectionX,carDirectionY;
long double carDirection;
long double lastTurningTime;
uint8_t firstTime = 1;
uint8_t _item_c_data_updated=0;

void useItem(int ID);

long double arctan(long double x, long double y){
	if(x==0){
		if(y>0){
			return pi/2;
		}
		return -pi/2;
	}
	long double res = atan(y/x);
	if(x<0){
		res = res + (res>0)?(-pi):pi;
	}
	return res;
}

void initItems(){
    int i;
    long double car_turned, tmp;
    int itemNum=((input_data.raw_data[1]>>3)&7);
    for(i=0;i<itemNum;i++){
        items[i].x=input_data.raw_data[11+2*i];
        items[i].y=input_data.raw_data[12+2*i];
        if(abs(xc-items[i].x)<8&&abs(yc-items[i].y)<8){
        	useItem(0x0e);
        }
    }
    for(;i<4;i++){
        items[i].x=-1;
        items[i].y=-1;
    }
    int ID=input_data.raw_data[0]>>6;
    if(firstTime){
        xl=input_data.raw_data[3+2*ID];
        yl=input_data.raw_data[4+2*ID];
        xc=xl;
        yc=yl;
        carDirectionX=0;
        carDirectionY=0;
        firstTime=0;
    }
    else{
        xl=xc;
        yl=yc;
        xc=input_data.raw_data[3+2*ID];
        yc=input_data.raw_data[4+2*ID];
        if((xc-xl)*(xc-xl)+(yc-yl)*(yc-yl)>=16) {
			car_turned=atan2(yc-yl, xc-xl) - atan2(carDirectionY, carDirectionX);
			if(car_turned < -pi){
				car_turned += 2*pi;
			}
			else if(car_turned > pi){
				car_turned -= 2*pi;
			}
			tmp = TurningTime*0.9+(abs(car_turned)/lastTurningTime)*0.1;
			if(tmp > 179104.4776 && tmp < 417910.4778){
				TurningTime = tmp;
			}
            carDirectionX = xc-xl;
            carDirectionY = yc-yl;
            _item_c_data_updated = 1;
        }
    }
}

// return value [0,180)
double angle(int x,int y){
    int targetDirectionX= x-xc, targetDirectionY=y-yc;
    if((carDirectionX==0&&carDirectionY==0)||(targetDirectionX==0&&targetDirectionY==0)) return 0;
    return acos(
    		(carDirectionX*targetDirectionX+carDirectionY*targetDirectionY)
			/(
					sqrt(carDirectionX*carDirectionX+carDirectionY*carDirectionY))
					*sqrt(targetDirectionX*targetDirectionX+targetDirectionY*targetDirectionY)
			)
    		/pi
			*180;
}

double Distance(int x,int y){
    return sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));
}

int firstItem(){
    int i;
    int FirstItem=-1;
    int minDistance=512;
    long double tmp;
    /*if(79<=xc&&xc<=176&&yc<=79){
    	for(i=0;i<4;i++){
    	    if(items[i].x==-1||items[i].y==-1) break;
            if(79<=items[i].x&&items[i].x<=176&&items[i].y<=79){
            if(Distance(items[i].x,items[i].y)<minDistance){
    	        FirstItem=i;
    	        minDistance=Distance(items[i].x,items[i].y);
    	        }
    	    }
    	}
        return FirstItem;
    }
    else if(79<=xc&&xc<=176&&176<=yc){
        	for(i=0;i<4;i++){
        	    if(items[i].x==-1||items[i].y==-1) break;
                if(79<=items[i].x&&items[i].x<=176&&176<=items[i].y){
                if(Distance(items[i].x,items[i].y)<minDistance){
        	        FirstItem=i;
        	        minDistance=Distance(items[i].x,items[i].y);
        	        }
        	    }
        }
        	return FirstItem;
    }
     else if(176<=xc&&79<=yc&&yc<=176){
            	for(i=0;i<4;i++){
            	    if(items[i].x==-1||items[i].y==-1) break;
                    if(176<=items[i].x&&79<=items[i].y&&items[i].y<=176){
                    if(Distance(items[i].x,items[i].y)<minDistance){
            	        FirstItem=i;
            	        minDistance=Distance(items[i].x,items[i].y);
            	        }
            	    }
            }
            	return FirstItem;
     }
      else if(xc<=79&&79<=yc&&yc<=176){
                	for(i=0;i<4;i++){
                	    if(items[i].x==-1||items[i].y==-1) break;
                        if(items[i].x<=79&&79<=items[i].y&&items[i].y<=176){
                        if(Distance(items[i].x,items[i].y)<minDistance){
                	        FirstItem=i;
                	        minDistance=Distance(items[i].x,items[i].y);
                	        }
                	    }
                }
                	return FirstItem;
      }*/
    for(i=0;i<4;i++){
        if(items[i].x==-1||items[i].y==-1) break;
        if(angle(items[i].x,items[i].y)<=limitAngle){
        	tmp = Distance(items[i].x,items[i].y);
            if(tmp < minDistance){
                FirstItem = i;
                minDistance = tmp;
            }
        }
    }
    return FirstItem;
}

int first_item_chushen(void){
    int i;
    int FirstItem=-1;
    int minDistance=512;
	if(79<=xc&&xc<=176&&yc<=79){
		for(i=0;i<4;i++){
			if(items[i].x==-1||items[i].y==-1) break;
			if(79<=items[i].x&&items[i].x<=176&&items[i].y<=79 && angle(items[i].x,items[i].y)<=limitAngle){
				if(Distance(items[i].x,items[i].y)<minDistance){
					FirstItem=i;
					minDistance=Distance(items[i].x,items[i].y);
				}
			}
		}
	}
	else if(79<=xc&&xc<=176&&176<=yc){
		for(i=0;i<4;i++){
			if(items[i].x==-1||items[i].y==-1) break;
			if(79<=items[i].x&&items[i].x<=176&&176<=items[i].y && angle(items[i].x,items[i].y)<=limitAngle){
				if(Distance(items[i].x,items[i].y)<minDistance){
					FirstItem=i;
					minDistance=Distance(items[i].x,items[i].y);
				}
			}
		}
	}
	else if(176<=xc&&79<=yc&&yc<=176){
		for(i=0;i<4;i++){
			if(items[i].x==-1||items[i].y==-1) break;
			if(176<=items[i].x&&79<=items[i].y&&items[i].y<=176 && angle(items[i].x,items[i].y)<=limitAngle){
				if(Distance(items[i].x,items[i].y)<minDistance){
					FirstItem=i;
					minDistance=Distance(items[i].x,items[i].y);
				}
			}
		}
	}
	else if(xc<=79&&79<=yc&&yc<=176){
		for(i=0;i<4;i++){
			if(items[i].x==-1||items[i].y==-1) break;
			if(items[i].x<=79&&79<=items[i].y&&items[i].y<=176 && angle(items[i].x,items[i].y)<=limitAngle){
				if(Distance(items[i].x,items[i].y)<minDistance){
					FirstItem=i;
					minDistance=Distance(items[i].x,items[i].y);
				}
			}
		}
    }
	return FirstItem;
}

void collectItems(int n){
    if(n==-1) return;
    /*for(j=0;j<SensorNum;j++){
        if(GetLightSensor(i)==0) break;
    }
    if(j<SensorNum) return;*/

    int targetX= items[n].x-xc, targetY=items[n].y-yc;

    //if(abs(targetX-xc)<=nearX&&abs(targetY-yc)<=nearY){
        //SetMotor(0,0);
        //SetMotor(1,0);
        //DELAY_US(ItemEatingTime);
        //return;
    //}

    if((carDirectionX==0&&carDirectionY==0)||(targetX==0&&targetY==0)) return;
    double angle1 = atan2(carDirectionY, carDirectionX) / pi * 180, angle2 = atan2(targetY, targetX) / pi * 180;
    unsigned char left;
    double Angle=angle(items[n].x,items[n].y);
    if(angle1<180){
        if(angle1<angle2 && angle2<180+angle1) left=false;
        else left=true;
    }
    else{
        if(angle1-180<angle2 && angle2<angle1) left=true;
        else left=false;
    }
    if(Angle < 30)
    	return;
    if(left){
        SetMotor(1,1000);
        SetMotor(0,-1000);
        lastTurningTime = TurningTime*Angle/90;
        DELAY_US(lastTurningTime);
    }
    else{
        SetMotor(0,1000);
        SetMotor(1,-1000);
        lastTurningTime = TurningTime*Angle/90;
        DELAY_US(lastTurningTime);
    }
}

void Items(void){
    if(input_data.raw_data[21] != 0x0d)
    	return;
    initItems();
    if(_item_c_data_updated){
    	_item_c_data_updated = 0;
    	collectItems(first_item_chushen());
    }
}
