#define pwm 70;




int main(){
	while(1){
		SetMotor(0,pwm);
		SetMotor(1,pwm);
		if(LightSensorFlag==1) FollowLine();	//�������жϣ���ѭ��
		Items();	//��ͼ�����
		Defend();	//����
	}
}