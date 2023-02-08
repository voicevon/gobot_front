#include "remotable_master_base.h"
#include "Mqtt/wifi_mqtt_client.h"

void RemoteVar_Masterbase::_PublishValue(char* new_value){
    g_mqttClient.publish(_mqtt_topic,2,true,_var_value);
}


