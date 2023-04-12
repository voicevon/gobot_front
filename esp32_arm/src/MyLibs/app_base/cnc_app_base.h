#pragma once


#include "Mqtt/mqtt_subscriber_base.h"
#include "CNC/gcode/gcode_queue.h"
#include "Mqtt/mqtt_message_queue.h"

// class CncAppBase: public GcodeProducer, public MqttSubscriberBase{
class CncAppBase: public MqttSubscriberBase{
    public:
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;
        void SpinOnce(); 

        // From remote mqtt to local gcode-queue
        void LinkQueues_Mqtt_to_Gcode(const char* mqtt_topic);
        // TODO:  This is a AppBase function,  
        //  The message payload will be a json string. like {'app':'reset'},  or {'gmcode':'M408'}
        //  will call virtual RunAppCommand() for app command
        //  will put to g_code_queue for gmcode.
        // void onGot_MqttMessage(const char* message_payload) override{
        //     command = message_payload
        //     if (to==app) {
        //         _RunAppCommand(command);
        //     } 
        //     if (to == gmcode) _gcode_queue->AppendGcodeCommand(command);
        // }
        // virtual void _RunAppCommand(const char* app_command){};

        bool CheckMqttCommand();
        GcodeQueue gcode_queue;
        MqttMessageQueue command_queue;

    protected:
        

    private:
        int test_id = 1234;

};