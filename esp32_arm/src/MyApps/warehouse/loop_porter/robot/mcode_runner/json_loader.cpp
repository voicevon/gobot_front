#include "json_loader.h"
#include "WString.h"
#include <ArduinoJson.h>
#include "MyLibs/utility/logger.h"
// #define PIN_IR_CHECKING 32    //   TODO:  This is should not be fixed,  For TWH project only,  right now.


char* Twh_LoopPorter_JsonLoader::Load(){
    // Logger::Debug("Twh_LoopPorter_JsonLoader::Load()");
    String str_state = "ready";
    if (__first_run) {
        str_state = "idle";
        __first_run = false;
    }
    str_state.toCharArray(__json_string, sizeof(str_state));
    // Logger::Print("json_string", __json_string);
    return __json_string;

    // TODO: feed back with message_id
    
    // const int capacity = JSON_OBJECT_SIZE(3);   //TODO JSON_OBJECT_SIZE = ?
    // StaticJsonDocument<capacity> doc;
    // // doc["topic"] = "twh/221109/r1/state";
    // doc["message_id"] = __message_id;
    // doc["is_moving"] = false;

    // serializeJson(doc, __json_string);
    // Logger::Print("json_string", __json_string);

    // __message_id++;

    // return __json_string;
}