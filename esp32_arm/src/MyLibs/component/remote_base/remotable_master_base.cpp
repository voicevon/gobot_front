#include "remotable_master_base.h"
#include "Mqtt/wifi_mqtt_client.h"

void RemoteVar_Masterbase::_PublishValue(const char* new_value){
    g_mqttClient.publish(_mqtt_topic, 2, true, new_value);
}


