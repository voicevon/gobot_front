#include "json_loader.h"
#include "WString.h"
#include <ArduinoJson.h>
#include "MyLibs/basic/logger.h"
#define PIN_IR_CHECKING 32    //   TODO:  This is should not be fixed,  For TWH project only,  right now.


char* Twh2_Row_JsonLoader::Load(){
    Logger::Debug("Twh2_Row_JsonLoader::Load()");
    const int capacity = JSON_OBJECT_SIZE(3);   //TODO JSON_OBJECT_SIZE = ?
    StaticJsonDocument<capacity> doc;
    // doc["topic"] = "twh/221109/r1/state";
    doc["id"] = __payload_id;
    doc["is_moving"] = false;

    serializeJson(doc, __json_string);
    Logger::Print("json_string", __json_string);

    __payload_id++;

    return __json_string;
}