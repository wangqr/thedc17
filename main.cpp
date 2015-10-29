#define pwm 70;




int main(){
	while(1){
		SetMotor(0,pwm);
		SetMotor(1,pwm);
		if(LightSensorFlag==1) FollowLine();	//若发生中断，则循迹
		Items();	//试图捡道具
		Defend();	//防御
	}
}