#include "ms5837.h"

#include "MyLibs/mqtt/wifi_mqtt_client.h"
// #include <Arduino.h>

#include "von/utility/mqtt/g_var.h"

#define MS5837_ADDRESS 0x76
#define MS5837_RESET 0X1e
#define MS5837_RESOLUTION_OF_4096 0Xff

void Ms5837::Init(TwoWire* i2c){
    __i2c = i2c;  // Init TwoWire* __i2c point to TwoWire(0) 

    __i2c->beginTransmission(MS5837_ADDRESS);
    __i2c->write(MS5837_RESET);
    __i2c->write(MS5837_RESOLUTION_OF_4096);
    __i2c->endTransmission();
// Wait for reset to complete
	delay(10);

}

void Ms5837::SpinOnce(){
    float sensor_value = __ReadSensor();
    if (sensor_value != __previous_sensor_value){
        // String payload="";
        // payload.concat(sensor_value);
        // g_mqttClient.publish(_mqtt_publish_topic, 2, true, payload.c_str());
        __previous_sensor_value = sensor_value;
    }
    // delay(10);
}


float Ms5837::__ReadSensor(){
    // return 1.234f;
    return __i2c->read();
}

float Ms5837::ReadSensor_data_debug(){

    float xx = __ReadSensor();
    return xx;


}
