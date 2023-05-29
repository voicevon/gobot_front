#include "remote_var_int.h"
#include "MyLibs/basic/logger.h"


void RemoteVar_Int::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    __remote_value = atoi(payload);
    __got_remote_value = true;
    Logger::Debug("ConcernSensorSetter::onGot_MqttMessage");
    Logger::Print(this->_mqtt_topic, payload);
    Logger::Print("__remote_value", __remote_value);
}

