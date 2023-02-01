#pragma once


#include "CNC/gcode/gcode_producer.h"
#include "Mqtt/mqtt_message_consumer.h"


class AppBase: public GcodeProducer, public MqttMessageConsumer{
    public:
        void SpinOnce(); 


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


    protected:
        

    private:

};