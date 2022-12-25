#include "twh2_m408_report_json_runner.h"
#include "IoT/wifi_mqtt_client.h"


#define PIN_IR_CHECKING 32    //   TODO:  This is should not be fixed,  For TWH project only,  right now.


// Mqtt publish a message, report something via MQTT
// The payload is a static buffer, declared as global.
bool Twh2_M408_Runner_ReportJson::RunnerLoop(){
    Logger::Debug("M408_Runner_ReportJson::RunnerLoop()");
    __payload_string = "blocked";
    if (digitalRead(PIN_IR_CHECKING) == HIGH) 
        __payload_string = "empty";   //TODO:  any memoory risk?
    const char* payload = __payload_string.c_str();
    Logger::Print(" M408_Runner_ReportJson::RunnerLoop()  payload", payload);
    g_mqttClient.publish("twh/221109/ir_state", 2, true, payload);

    return true;   
}