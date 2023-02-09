#include "remotable_master_base.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "MyLibs/basic/logger.h"

void RemoteVar_Masterbase::_PublishValue(const char* new_value){
    Logger::Debug("RemoteVar_Masterbase::_PublishValue");
    Logger::Print("_mqtt_topic",_mqtt_topic);
    Logger::Print("new_value", new_value);
    g_mqttClient.publish(_mqtt_topic, 2, true, new_value);
}


