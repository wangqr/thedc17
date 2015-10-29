#define pwm 70
#define delta_pwm1 10
#define delta_pwm2 20


void turn(bool direction,bool level){    //����ת�����
	while(GetLightSensor(0)==0||GetLightSensor(1)==0||GetLightSensor(2)==0||GetLightSensor(3)==0){
		int delta_power;
		if(level==0) delta_power=delta_pwm1;
		else delta_power=delta_pwm2;
		if(direction==0){
			SetMotor(1,pwm-delta_power);
			SetMotor(0,pwm+delta_power);
		}
		else{
			SetMotor(0,pwm-delta_power);
			SetMotor(1,pwm+delta_power);
		}
	}
	LightSensorFlag=0;
}

void FollowLine(){                        //Ѳ��
	if(GetLightSensor(1)==0){
		turn(0,1);
	}
	else if(GetLightSensor(0)==0){
		turn(0,0);
	}
	if(GetLightSensor(2)==0){
		turn(1,1);
	}
	else if(GetLightSensor(3)==0){
		turn(1,0);
	}
}

