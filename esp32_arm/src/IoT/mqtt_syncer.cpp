#include "mqtt_syncer.h"

void MqttSyncer::LinkLocalCommandQueue_AsMqttMessageProducer(MessageQueue* loacalMQ){
    this->__localMQ = loacalMQ;
}

void MqttSyncer::SubscribeMqtt(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic){
    this->__mqttClient = mqttClient;
    mqttClient->subscribe(main_topic, 2);
    // mqttClient->subscribe("gobot/x2134/house", 2);
    this->topic_feedback = String(feedback_topic);
    // this->topic_feedback = "gobot/x2134/house/fb";

    bool debug=true;
    if(debug){
        Serial.print("\nMqttSyncer::SubscribeMqtt() is done  Subsribe_topic= ");
        Serial.print(main_topic);
        Serial.print(" feedback_topic= ");
        Serial.println(feedback_topic);
    }
}


void MqttSyncer::OnReceived(const char* payload, int length){
    // Put message to local MQ   
    // TODO:: Is this necessary??
    char* p = (char*)(payload) + length;
    *p = 0x00;

    this->__local_mq_is_full = this->__localMQ->AppendMessage(payload, length); 

    // send message to feedback topic
    if (this->__local_mq_is_full){
        Serial.print("\nMqttSyncer::OnReceived() is appended to local mq, will not publish a feedback via mqtt.");
        return;
    }
    //  local message queue is not full [after appending current message], publish mqtt feedback now.
    this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, payload, length);
}

// This function will be invoked on master thread.
// Check if local mq is:  from full to not.
void MqttSyncer::SpinOnce(){
    if (this->__local_mq_is_full){
        // double check whether local mq is still full or not.
        Serial.println("[Info] MqttSyncer::SpinOnce()  __local_mq_is_full");
        if (! this->__localMQ->BufferIsFull()){
            // the local mq is not full right now. publish a feedback.
            Serial.println("\n                  MqttSyncer::SpinOnce() local mq got a free room");
            MessageQueue::SingleMessage* pMessage = this->__localMQ->GetHeadMessage();
            this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, pMessage->payload, pMessage->length);
            this->__local_mq_is_full = false;
        }
    }
}