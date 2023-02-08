#pragma once

#include "MyLibs/basic/json_loader_base.h"

class LoopPorter_JsonLoader: public JsonLoaderBase{
    public:
        char* Load() override;

    private:
        char __json_string[1000];
        int __message_id = 0;
        bool __first_run = true;
};