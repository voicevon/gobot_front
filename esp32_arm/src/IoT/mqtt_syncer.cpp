#include "mqtt_syncer.h"

void MqttSyncer::LinkLocalCommandQueue_AsMqttMessageProducer(MessageQueue* loacalMQ){
    this->__localMQ = loacalMQ;
}

void MqttSyncer::SubscribeMqtt(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic){
    this->__mqttClient = mqttClient;
    mqttClient->subscribe(main_topic, 2);
    this->topic_feedback = String(feedback_topic);

    bool debug=true;
    if(debug){
        Logger::Debug("MqttSyncer::SubscribeMqtt() is done");
        Serial.print("Subsribe_topic= ");
        Serial.print(main_topic);
        Serial.print(" feedback_topic= ");
        Serial.println(feedback_topic);
    }
}


void MqttSyncer::OnReceived(const char* payload, int length){
    // Put message to local MQ   
    // TODO:: Is this necessary??
    // Logger::Info("MqttSyncer::OnReceived() Starting a huge process...");
    char* p = (char*)(payload) + length;
    *p = 0x00;

    this->__local_mq_is_full = this->__localMQ->AppendMessage(payload, length); 

    // send message to feedback topic
    if (this->__local_mq_is_full){
        Logger::Info("MqttSyncer::OnReceived() is appended to local mq, will not publish a feedback via mqtt.");
        return;
    }
    //  local message queue is not full [after appending current message], publish mqtt feedback now.
    // Serial.println("[Info] MqttSyncer::OnReceived() sending feedback.");
    this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, payload, length);
    // Serial.println("[Info] MqttSyncer::OnReceived() sent feedback.");

}

// This function will be invoked on master thread.
// Check whether local mq is full:  from full to not.
void MqttSyncer::SpinOnce(){
    if (this->__local_mq_is_full){
        // double check whether local mq is still full or not.
        // Serial.println("[Info] MqttSyncer::SpinOnce()  __local_mq_is_full");
        if (! this->__localMQ->BufferIsFull()){
            // the local mq is not full right now. publish a feedback.
            Serial.println("\n                  MqttSyncer::SpinOnce() local mq got a free room");
            MessageQueue::SingleMessage* pMessage = this->__localMQ->GetHeadMessage();
            this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, pMessage->payload, pMessage->length);
            this->__local_mq_is_full = false;
        }
    }
}