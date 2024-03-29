
#include "m408_runner_mqtt_publish.h"
// #include "von/utility/mqtt/task_mqtt.h"
#include "von/utility/mqtt/g_var.h"



void M408_Runner_MqttPublish::SetupRunner(GcodeText* mcode_text){
    GcodeHelper gcode_helper = GcodeHelper(mcode_text->GetChars);
    GcodeHelper* mcode = &gcode_helper;
    // Logger::Info("M408_Runner_ReportJson::SetupRunner()");
    strcpy(__publish_topic_string, mcode->get_command());
    // mcode_text->CopyTo(__publish_topic_string);   //this will let __publish_topic_string == "M408twh/221109/r1/state"
    // Logger::Print(" 408_Runner_MqttPublish::SetupRunner()   __publish_topic_string", __publish_topic_string);
}

bool M408_Runner_MqttPublish::RunnerLoop(){
    // Logger::Debug("M408_Runner_MqttPublish::RunnerLoop()");
    char* payload = __json_loader->Load();
    // Logger::Print("__publish_topic_string", __publish_topic_string);
    // Logger::Print("Loaded json string", payload);

    g_mqttClient.publish(__publish_topic_string, 2, true, payload);
    return true;
}
