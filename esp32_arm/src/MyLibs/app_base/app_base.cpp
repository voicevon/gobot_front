#include "app_base.h"
#include "MyLibs/mqtt/mqtt_subscriber_manager.h"

#define APP_COMMAND_PREFIX  "app:"
#define APP_COMMAND_PREFIX_SIZE 4


void AppBase::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    //format string ender.
    char* p = (char*)(payload);
    p[payload_len] = 0x00;  
    Logger::Debug("AppBase::onGot_MqttMessage");
    Logger::Print("payload", payload);
    Logger::Print("payload_len", payload_len);
    // const char* IS_APP_COMMAND = APP_COMMAND_PREFIX;

    __have_done_feedback = false;
    // TextMessageLine command_text;
    __app_command.CopyFrom(payload,payload_len);
    if (__app_command.IsEqualTo("app:led")){
        // This is a thread in mqtt-on-received callbaking.    so watchdog will be fired if long time without return.
        // this->ExecuteAppCommand(&__app_command);
        return;
    }
    __app_command.CopyFrom("none");
    int free_buffer_count = _text_message_queue->GetFreeBuffersCount();
    if (free_buffer_count <=1 ){
        Logger::Error("AppBase::onGot_MqttMessage()  buffer is full");
        return;
    }
    // copy to local _text_message_queue
    this->_text_message_queue->AppendCommand(payload, payload_len);
}

void AppBase::Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic, TextMessageQueue* text_message_queue){
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

    this->_text_message_queue = text_message_queue;  
    gs_MqttSubscriberManager::Instance().AddSubscriber(mqtt_topic, this);
    
    // Logger::Print("AppBase::Link_Mqtt_to_TextMessageQueue()   mqtt_topic", mqtt_topic);
    // Logger::Print("AppBase::Link_Mqtt_to_TextMessageQueue()  this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
}

void AppBase::SpinOnce(){
    if (__have_done_feedback)
        return;
    if (__app_command.IsPrefix("app:")){
        g_mqttClient.publish(this->__mqtt_topic_feedback, 2, true, __app_command.GetChars());
        this->ExecuteAppCommand(&__app_command);
    }
    if (_text_message_queue->GetFreeBuffersCount() == 0)
        return;

    // Logger::Debug("AppBase::SpinOnce()   send_feedback, started");
    TextMessageLine* command_text = _text_message_queue->GetDepositHeadElement();

    // Logger::Print("send_feedback, this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
    // Logger::Print("send_feedback, payload", command_text->GetChars);
    // command_text->PrintFlat("verify origin");
    g_mqttClient.publish(this->__mqtt_topic_feedback, 2, true, command_text->GetChars());
    __have_done_feedback = true;
    // Logger::Print("send_feedback, finished.", 99);
}



