#pragma once
#include "MyLibs/app_base/text_message_queue/text_message_line.h"
#include <SPIFFS.h>


class FileWritter{
    public:
        void FeedText(TextMessageLine* text);

    private:
        File __file;


};