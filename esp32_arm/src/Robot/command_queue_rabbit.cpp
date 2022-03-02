#include "command_queue_rabbit.h"

void CommandQueueRabbit::LinkLocalCommandQueue(CommandQueue* loacalMQ){
    this->__localMQ = loacalMQ;
}

void CommandQueueRabbit::SubscribeMqtt(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic){
    this->__mqttClient = mqttClient;
    mqttClient->subscribe(main_topic, 2);
    // mqttClient->subscribe("gobot/x2134/house", 2);
    this->topic_feedback = String(feedback_topic);
    // this->topic_feedback = "gobot/x2134/house/fb";

    bool debug=true;
    if(debug){
        Serial.print("\nCommandQueueRabbit::SubscribeMqtt() is done    ");
        Serial.print(main_topic);
        Serial.print("      ");
        Serial.println(feedback_topic);
    }
}


void CommandQueueRabbit::OnReceived(char* payload, int length){
    // Serial.println("CommandQueueRabbit::OnReceived() is entering...");
    // Put message to local MQ
    String strPayload = payload;
    // this->__localMQ->SayHello();
    this->__localMQ->AppendGcodeCommand(strPayload); 
    // send message to feedback topic
    this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, payload, length);
    // Serial.println("CommandQueueRabbit::OnReceived() is exiting...");
}