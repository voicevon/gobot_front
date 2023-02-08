#pragma once

#include "MyLibs/basic/json_loader_base.h"

class TwhPacker_JsonLoader: public JsonLoaderBase{
    public:
        char* Load() override;

    private:
        char __json_string[1000];
};