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
    int free_buffer_count = gcode_queue.GetFreeBuffersCount();
    if (free_buffer_count <=1 ){
        Logger::Error("CncAppBase::onGot_MqttMessage()  buffer is full");
        return;
    }
    // copy to local gcode_queue.
    this->gcode_queue.AppendGcodeCommand(payload, payload_len);
}

void CncAppBase::Link_Mqtt_to_GcodeQueue(const char* mqtt_topic){
    Logger::Print("CncAppBase::Link_Mqtt_to_GcodeQueue()   mqtt_topic", mqtt_topic);
    // construct feedback mqtt_topic
    char * p = (char*) (this->__mqtt_topic_feedback);
    int topic_len;
    for(topic_len=0; topic_len<REPRAP_GCODE_MAX_SIZE; topic_len++){
        Logger::Print("mqtt_topic[topic_len]", mqtt_topic[topic_len]);
        if (mqtt_topic[topic_len] == 0x00){
            break;
        }
        // p[topic_len] = mqtt_topic[topic_len];
    }
    p[topic_len] = '/';
    p[topic_len+1] = 'f';
    p[topic_len+2] = 'b';
    p[topic_len+3] = 0x00;   //ender
    Logger::Print("CncAppBase::Link_Mqtt_to_GcodeQueue()  this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
    
    gs_MqttSubscriberManager::Instance().AddSubscriber(mqtt_topic, this);
}

void CncAppBase::SpinOnce(){
    if (this->test_id != 1234){
        Logger::Warn("CncAppBase::SpinOnce(),  The bug is eating.!!!!");
        Logger::Halt("BUG!");
    }
    // Logger::Debug("CncAppBase::SpinOnce()");
    // Logger::Print("this->test_id", this->test_id);
    // this->gcode_queue.SayHello("caller is :  CncAppBase::SpinOnce()");
    if (__have_done_feedback)
        return;
    if (gcode_queue.GetFreeBuffersCount() == 0)
        return;

    // send_mqtt_feedback 
    const char * payload = gcode_queue.GetDepositHeadElement()->bytes;

    Logger::Print("send_feed_back, this->_mqtt_topic", this->_mqtt_topic);
    Logger::Print("send_feed_back, payload", payload);
    g_mqttClient.publish(this->__mqtt_topic_feedback, 2, true, payload );
    __have_done_feedback = true;

}



