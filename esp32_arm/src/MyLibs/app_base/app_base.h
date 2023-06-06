#pragma once

#include "MyLibs/mqtt/mqtt_subscriber_base.h"
#include "CNC/gcode/gcode_queue.h"
#include "message_queue/message_queue.h"

class AppBase: public MqttSubscriberBase{
    public:
        void Link_Mqtt_to_CommandQueue(const char* mqtt_topic, CommandQueue* gcode_queue);
        virtual void ExecuteCommand(CommandText* gcode_text){}; 

        void SpinOnce(); 
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;


    protected:
        CommandQueue* _command_queue;
        

    private:
        int test_id = 1234;
        bool __have_done_feedback = true;
        char __mqtt_topic_feedback[40];
        CommandText __app_command;
};