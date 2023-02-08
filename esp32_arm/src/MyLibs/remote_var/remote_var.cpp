#include "remote_var.h"
#include "wifi_mqtt_client.h"


RemoteVar_via_Mqtt::RemoteVar_via_Mqtt(const char* matt_topic){
    __mqtt_topic = __mqtt_topic;
}

void RemoteVar_via_Mqtt::Start(){
    g_mqttClient.subscribe(__mqtt_topic,2);
    // g_mqttClient.onMessage(aaa);    //static callback  none  static ??
}
