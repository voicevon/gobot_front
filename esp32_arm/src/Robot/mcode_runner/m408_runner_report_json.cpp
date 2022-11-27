
#include "m408_runner_report_json.h"
#include "Robot/axis_homer/home_trigger_array.h"

#include "IoT/wifi_mqtt_client.h"


// Mqtt publish a message, report something via MQTT
// The payload is a static buffer, declared as global.
bool M408_Runner_ReportJson::StartExec(Gcode* gcode){
    Logger::Debug("M408_Runner_ReportJson::Run()");

    char *payload = (char*) (gcode->get_command());
    mqttClient.publish("twh/221109/sensors",2,true, payload);

    return true;   
}