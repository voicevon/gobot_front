#pragma once
#include "mcode_runner_base.h"
#include  "MyLibs/basic/json_loader_base.h"


#define MQTT_PAYLOAD_BUFFER_COUNT_1K 1000
#define MQTT_PAYLOAD_BUFFER_COUNT_200K 200000    // For camera picture.
class M408_Runner_MqttPublish: public McodeRunnerBase{
    public:
        void LinkJsonLoader(JsonLoaderBase* json_loader){this->__json_loader = json_loader;};
        void SetupRunner(Gcode* mcode) override;
        bool RunnerLoop() override;

    protected:

        // TODO::   zero-copy of the buffer.
        char __topic_string[50];
        char __payload_buffer[MQTT_PAYLOAD_BUFFER_COUNT_1K];
        String __payload_string;
        JsonLoaderBase* __json_loader;

};
