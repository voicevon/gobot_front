
#include "m408_runner_mqtt_publish.h"
#include "Mqtt/wifi_mqtt_client.h"


void M408_Runner_MqttPublish::SetupRunner(GcodeText* mcode_text){
    GcodeHelper gcode_helper = GcodeHelper(mcode_text->bytes);
    GcodeHelper* mcode = &gcode_helper;
    Logger::Info("M408_Runner_ReportJson::SetupRunner()");
    strcpy(__topic_string, mcode->get_command());
    Logger::Print("topic", __topic_string);
}

bool M408_Runner_MqttPublish::RunnerLoop(){
    Logger::Debug("M408_Runner_MqttPublish::RunnerLoop()");
    char* payload = __json_loader->Load();
    Logger::Print("topic", __topic_string);
    Logger::Print("Loaded json string", payload);

    // g_mqttClient.publish("twh/221109/r1/state", 2, true, payload);
    g_mqttClient.publish(__topic_string, 2, true, payload);
    return true;
}
