#pragma once

#include "MyLibs/basic/json_loader_base.h"

class Twh2_JsonLoader: public JsonLoaderBase{
    public:
        char* Load() override;

    private:
        char __json_string[1000];
};