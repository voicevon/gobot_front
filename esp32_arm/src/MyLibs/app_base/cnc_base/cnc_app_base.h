#pragma once


#include "von/cpp/mqtt/subscriber/mqtt_subscriber_base.h"
#include "CNC/gcode/gcode_queue.h"

class CncAppBase: public MqttSubscriberBase{
    public:
        void Link_Mqtt_to_GcodeQueue(const char* mqtt_topic, GcodeQueue* gcode_queue);
        // virtual void ExecuteAppCommand(GcodeText* gcode_text){}; 

        void SpinOnce(); 
        void onGot_MqttMessage_whole(const char* payload, size_t payload_len) override ;


    protected:
        GcodeQueue* _gcode_queue;
        

    private:
        int test_id = 1234;
        bool __have_done_feedback = true;
        char __mqtt_topic_feedback[40];
        GcodeText __app_command;
};