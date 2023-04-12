#pragma once


#include "Mqtt/mqtt_subscriber_base.h"
#include "CNC/gcode/gcode_queue.h"
#include "Mqtt/mqtt_message_queue.h"

// class CncAppBase: public GcodeProducer, public MqttSubscriberBase{
class CncAppBase: public MqttSubscriberBase{
    public:
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;
        void SpinOnce(); 
        void Link_Mqtt_to_GcodeQueue(const char* mqtt_topic);

        GcodeQueue gcode_queue;
        // MqttMessageQueue command_queue;
        virtual void ExecuteCommand(const char* command){};

    protected:
        

    private:
        int test_id = 1234;
        void __send_mqtt_feedback();
        bool __have_done_feedback = true;

};