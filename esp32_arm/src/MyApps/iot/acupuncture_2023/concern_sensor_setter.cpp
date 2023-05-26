#include "concern_sensor_setter.h"
#include "MyLibs/basic/logger.h"
// #include "MyLibs/basic/memory_helper.h"


void ConcernSensorSetter::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    __remote_value = atoi(payload);
    __got_remote_value = true;
    Logger::Debug("ConcernSensorSetter::onGot_MqttMessage");
    Logger::Print(this->_mqtt_topic, payload);
    Logger::Print("__remote_value", __remote_value);
}

