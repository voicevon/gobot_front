#pragma once


// #include "Mqtt/mqtt_subscriber_base.h"
#include "MyLibs/mqtt/mqtt_subscriber_base.h"
#include "CNC/gcode/gcode_queue.h"

class CncAppBase: public MqttSubscriberBase{
    public:
        void Link_Mqtt_to_GcodeQueue(const char* mqtt_topic, GcodeQueue* gcode_queue);
        virtual void ExecuteCommand(GcodeText* gcode_text){}; 

        void SpinOnce(); 
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;


    protected:
        GcodeQueue* _gcode_queue;
        

    private:
        int test_id = 1234;
        bool __have_done_feedback = true;
        char __mqtt_topic_feedback[40];
        GcodeText __app_command;
};