#include "json_loader.h"
#include "WString.h"
#include <ArduinoJson.h>
#include "MyLibs/basic/logger.h"
#define PIN_IR_CHECKING 32    //   TODO:  This is should not be fixed,  For TWH project only,  right now.


char* Twh2_Row_JsonLoader::Load(){
    Logger::Debug("Twh2_Row_JsonLoader::Load()");
    String idle = "ready";
    idle.toCharArray(__json_string, sizeof(idle));
    Logger::Print("json_string", __json_string);
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