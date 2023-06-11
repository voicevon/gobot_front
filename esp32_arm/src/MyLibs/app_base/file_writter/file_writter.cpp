#include "file_writter.h"

void FileWritter::FeedText(TextMessageLine* text){
    if (text->IsPrefix("file_open")){
        text->RemovePrefix(':');
        __file = SPIFFS.open(text->c_str());

    }else if (text->IsPrefix("file_close")){
        __file.close();
    }else if(text->IsPrefix("file_line")){
        text->RemovePrefix(':');
        text->Concat("\n");
        __file.write((unsigned char*) text->c_str(),text->GetStringLength());

    }else{
        // write text to file
    }
}
