#include "remote_queue_bridge_mqtt.h"

void RemoteQueueBridge_mqtt::LinkLocalCommandQueue_AsMqttMessageProducer(MqttMessageQueue* loacalMQ){
    this->__localMQ = loacalMQ;
}

// void RemoteQueueBridge_mqtt::Init(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic){
void RemoteQueueBridge_mqtt::Init(const char* feedback_topic){
    // this->__mqttClient = mqttClient;
    // mqttClient->subscribe(main_topic, 2);
    __feedback_topic = String(feedback_topic);

    bool debug=true;
    if(debug){
        Logger::Debug("RemoteQueueBridge_mqtt::SubscribeMqtt() is done");
        // Serial.print("Subsribe_topic= ");
        // Serial.print(main_topic);
        Serial.print(" feedback_topic= ");
        Serial.println(feedback_topic);
    }
}


// void RemoteQueueBridge_mqtt::onMessage(const char* payload, uint16_t length){
//     // Put message to local MQ   
//     // TODO:: Copy char array simpler way.
//     Logger::Info("RemoteQueueBridge_mqtt::OnReceived() Starting a huge process...");
//     char* p = (char*)(payload) + length;
//     *p = 0x00;


//     // json decoder to get 
//     //  paylaod. id     is an int.
//     //  payload. key    is a string;
//     //  payload. value  is a string.

//     // __newest_payload_id = 123;
//     // String key_value = "cmd: beep";
//     // __local_mq_is_full = __localMQ->AppendMessage(key_value); 
//     __local_mq_is_full = __localMQ->AppendMessage(payload, length); 

//     // send message to feedback topic
//     if (this->__local_mq_is_full){
//         Logger::Info("RemoteQueueBridge_mqtt::OnReceived() is appended to local mq, will not publish a feedback via mqtt.");
//         return;
//     }

    // convert mqtt_payload to bridge_message


    //  local message queue is not full [after appending current message], publish mqtt feedback now.
    // Serial.println("[Info] RemoteQueueBridge_mqtt::OnReceived() sending feedback.");
    // int payload_id = 123;   // union structure ?
    // payload = (const char*)(payload_id);  //?? ender of string ??
    // String newest_payload_id = String(__newest_payload_id);
    // g_mqttClient.publish(this->__feedback_topic.c_str(), 2, true, payload, length);
    // Serial.println("[Info] RemoteQueueBridge_mqtt::OnReceived() sent feedback.");

// }

// This function will be invoked on master thread.
// Check whether local mq is full:  from full to not.
void RemoteQueueBridge_mqtt::SpinOnce(){
    if (this->__local_mq_is_full){
        // double check whether local mq is still full or not.
        // Serial.println("[Info] RemoteQueueBridge_mqtt::SpinOnce()  __local_mq_is_full");
        if (! this->__localMQ->BufferIsFull()){
            // the local mq is not full right now. publish a feedback.
            Serial.println("\n                  RemoteQueueBridge_mqtt::SpinOnce() local mq got a free room");
            MqttMessage* pMessage = this->__localMQ->GetHeadMessage();
            g_mqttClient.publish(__feedback_topic.c_str(), 2, true, pMessage->payload, pMessage->length);
            this->__local_mq_is_full = false;
        }
    }
}