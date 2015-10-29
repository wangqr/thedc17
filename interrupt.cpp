bool LightSensorFlag;

void LightSensorInterrupt(){   //发生中断后把标识设为1
	LightSensorFlag=1;
}
