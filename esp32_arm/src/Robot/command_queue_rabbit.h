/*
.                                      |----------  MyLastedMessage <-----|
GobotHead -------> HouseQueue -----> Agent  -------> HouseMQTT -------> HouseBot
            |
            |----> ArmQueue 
.




*/

#include <Arduino.h>
#include "command_queue.h"

class CommandQueueRabbit{
    public:
        String topic;
        String topic_feedback;
        byte* payload;
        void Init(CommandQueue* loacalMQ, String mqtt_topic);
        void OnReceived(byte* meeeage);
        
    private:
        CommandQueue* __localMQ;
};