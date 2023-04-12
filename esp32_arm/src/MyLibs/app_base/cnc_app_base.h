#pragma once


#include "Mqtt/mqtt_subscriber_base.h"
#include "CNC/gcode/gcode_queue.h"
// #include "Mqtt/mqtt_message_queue.h"

class CncAppBase: public MqttSubscriberBase{
    public:
        void Link_Mqtt_to_GcodeQueue(const char* mqtt_topic);
        virtual void ExecuteCommand(const char* command){};  // TODO:  remove the defination

        void SpinOnce(); 
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;
        // void Init(const char* mqtt_topic, cncro)

        GcodeQueue gcode_queue;

    protected:
        

    private:
        int test_id = 1234;
        bool __have_done_feedback = true;
        // REPRAP_GCODE_MAX_SIZE
        char* __mqtt_topic_feedback = "12345678901234567890123456789012345";   // length equal to  REPRAP_GCODE_MAX_SIZE
        // char __mqtt_topic_feedback =  char[40];
};