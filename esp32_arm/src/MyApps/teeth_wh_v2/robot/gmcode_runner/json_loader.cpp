#include "json_loader.h"
#include "WString.h"

char* Twh2_JsonLoader::Load(){
    String a ="aaaaaaaaa";
    // copy string to chars
    a.getBytes((unsigned char*)__json_string, a.length());
    return __json_string;
}