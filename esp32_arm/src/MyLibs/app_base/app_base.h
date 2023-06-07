#pragma once

#include "MyLibs/mqtt/mqtt_subscriber_base.h"
#include "CNC/gcode/gcode_queue.h"
#include "text_message_queue/text_message_queue_dispach.h"

class AppBase: public MqttSubscriberBase{
    public:
        void Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic, TextMessageQueue* text_command_queue);
        virtual void ExecuteAppCommand(TextMessageLine* gcode_text){}; 

        void SpinOnce(); 
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;


    protected:
        TextMessageQueue* _text_message_queue;
        

    private:
        int test_id = 1234;
        bool __have_done_feedback = true;
        char __mqtt_topic_feedback[40];
        TextMessageLine __app_command;
};