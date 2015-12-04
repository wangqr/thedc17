
#include "th_sensor.h"

void(*_th_sensor_callback)();
const GPIO_Number_e _th_light_sensor_pin[] = { GPIO_Number_6, GPIO_Number_7, GPIO_Number_16, GPIO_Number_17 };

void init_sensor() {
    uint8_t i;

    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_1, PIE_SubGroupNumber_4,
        (intVec_t)&xint1_isr);
    PIE_enableInt(myPie, PIE_GroupNumber_1, PIE_InterruptSource_XINT_1);

    // Enable INT1 which is connected to WAKEINT
    CPU_enableInt(myCpu, CPU_IntNumber_1);

    // Initialize GPIO
    for (i = 0; i < _th_light_sensor_cnt; ++i) {
        GPIO_setPullUp(myGpio, _th_light_sensor_pin[i], GPIO_PullUp_Disable);
        GPIO_setQualification(myGpio, _th_light_sensor_pin[i], GPIO_Qual_ASync);
        GPIO_setMode(myGpio, _th_light_sensor_pin[i], (GPIO_Mode_e)0);
        GPIO_setDirection(myGpio, _th_light_sensor_pin[i], GPIO_Direction_Input);
        GPIO_setExtInt(myGpio, _th_light_sensor_pin[i], CPU_ExtIntNumber_1);
    }

    // Configure XINT1
    PIE_setExtIntPolarity(myPie, CPU_ExtIntNumber_1,
        PIE_ExtIntPolarity_RisingEdge);

    // Enable XINT1 and XINT2
    PIE_enableExtInt(myPie, CPU_ExtIntNumber_1);
}

void th_sensor_set_callback(void(*f)()) {
    _th_sensor_callback = f;
}

uint8_t GetLightSensor(uint8_t index) {
    return GPIO_getData(myGpio, _th_light_sensor_pin[index]);
}

__interrupt void xint1_isr(void) {
    //if (_th_sensor_callback != NULL){
        //_th_sensor_callback();
    //}
    PIE_clearInt(myPie, PIE_GroupNumber_1);
}
