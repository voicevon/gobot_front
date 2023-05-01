#include "ms5837.h"
#include "Mqtt/wifi_mqtt_client.h"



void Ms5837::Init(){

}

void Ms5837::SpinOnce(){
    float sensor_value = __GetDepth_mm();
    if (sensor_value != __previous_sensor_value){
        g_mqttClient.publish(_mqtt_publish_topic, 2, true, sensor_value);
        __previous_sensor_value = sensor_value;
    }
}


float Ms5837::__GetDepth_mm(){
    return 1.234;
}

