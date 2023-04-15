#include "cnc_app_base.h"
#include "Mqtt/mqtt_subscriber_manager.h"

#define APP_COMMAND_PREFIX  "app:"
#define APP_COMMAND_PREFIX_SIZE 4


void CncAppBase::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    //format string ender.
    char* p = (char*)(payload);
    p[payload_len] = 0x00;  
    Logger::Debug("CncAppBase::onGot_MqttMessage");
    Logger::Print("payload", payload);
    Logger::Print("payload_len", payload_len);
    const char* IS_APP_COMMAND = APP_COMMAND_PREFIX;

    __have_done_feedback = false;

    bool is_app_command = true;
    for (int i=0; i<APP_COMMAND_PREFIX_SIZE; i++){
        if (payload[i] != IS_APP_COMMAND[i]){
            is_app_command = false;
            break;
        }
    }

    if (is_app_command){
        this->ExecuteCommand(payload);
        // __send_mqtt_feedback();
        return;
    }
    // check gcode_queue buffer size.
    int free_buffer_count = _gcode_queue->GetFreeBuffersCount();
    if (free_buffer_count <=1 ){
        Logger::Error("CncAppBase::onGot_MqttMessage()  buffer is full");
        return;
    }
    // copy to local _gcode_queue
    this->_gcode_queue->AppendGcodeCommand(payload, payload_len);
}

void CncAppBase::Link_Mqtt_to_GcodeQueue(const char* mqtt_topic, GcodeQueue* gcode_queue){
    // construct feedback mqtt_topic
    int topic_len;
    for(topic_len=0; topic_len<REPRAP_GCODE_MAX_SIZE; topic_len++){
        // Logger::Print("mqtt_topic[topic_len]", mqtt_topic[topic_len]);
        if (mqtt_topic[topic_len] == 0x00){
            break;
        }
        __mqtt_topic_feedback[topic_len] = mqtt_topic[topic_len];
    }
    __mqtt_topic_feedback[topic_len] = '/';
    __mqtt_topic_feedback[topic_len+1] = 'f';
    __mqtt_topic_feedback[topic_len+2] = 'b';
    __mqtt_topic_feedback[topic_len+3] = 0x00;   //ender

    this->_gcode_queue = gcode_queue;  
    gs_MqttSubscriberManager::Instance().AddSubscriber(mqtt_topic, this);
    
    // Logger::Print("CncAppBase::Link_Mqtt_to_GcodeQueue()   mqtt_topic", mqtt_topic);
    // Logger::Print("CncAppBase::Link_Mqtt_to_GcodeQueue()  this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
}

void CncAppBase::SpinOnce(){
    if (this->test_id != 1234){
        Logger::Warn("CncAppBase::SpinOnce(),  The bug is eating.!!!!");
        Logger::Halt("BUG!");
    }
    if (__have_done_feedback)
        return;
    if (_gcode_queue->GetFreeBuffersCount() == 0)
        return;

    // Logger::Debug("CncAppBase::SpinOnce()   send_feedback, started");
    GcodeText* gcode_text = _gcode_queue->GetDepositHeadElement();

    // Logger::Print("send_feedback, this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
    // Logger::Print("send_feedback, payload", gcode_text->GetChars);
    // gcode_text->PrintFlat("verify origin");
    g_mqttClient.publish(this->__mqtt_topic_feedback, 2, true, gcode_text->GetChars);
    __have_done_feedback = true;
    // Logger::Print("send_feedback, finished.", 99);
}



