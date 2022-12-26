
#include "m408_runner_mqtt_publish.h"
#include "Robot/axis_homer/position_trigger_array.h"
// #include "Robot/global_vars.h"
#include "IoT/wifi_mqtt_client.h"


void M408_Runner_MqttPublish::SetupRunner(Gcode* mcode){
    // Logger::Info("M408_Runner_ReportJson::SetupRunner");
    strcpy(__topic_string, mcode->get_command());
}

bool M408_Runner_MqttPublish::RunnerLoop(){
    char* payload = __json_loader->Load();
    g_mqttClient.publish("topic", 2, true, payload);
    return true;
}
