#include "ms5837.h"
#include "von/cpp/mqtt/task_mqtt.h"



void Ms5837::Init(){

}

void Ms5837::SpinOnce(){
    float sensor_value = __ReadSensor();
    if (sensor_value != __previous_sensor_value){
        String payload="";
        payload.concat(sensor_value);
        g_mqttClient.publish(_mqtt_publish_topic, 2, true, payload.c_str());
        __previous_sensor_value = sensor_value;
    }
}


float Ms5837::__ReadSensor(){
    return 1.234f;
}

