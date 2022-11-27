
#include "m408_runner_report_json.h"
#include "Robot/axis_homer/home_trigger_array.h"

#include "IoT/wifi_mqtt_client.h"


// Mqtt publish a message, report something via MQTT
bool M408_Runner_ReportJson::StartToRun(Gcode* gcode){
    Logger::Debug("M119_Runner::Run()");
    static uint32_t last_flags;
    // HomeTrigger_Array::Instance().GetFiredPosition('X');
    uint32_t flags = HomeTrigger_Array::Instance().GetStateBitsFlag();
    if (flags != last_flags){
        Serial.print(flags,BIN);
        Logger::Print("\t\tflags", flags);
        char *payload = (char*) (gcode->get_command());
        mqttClient.publish("twh/221109/sensors",2,true, payload);

    }
    return true;   
}