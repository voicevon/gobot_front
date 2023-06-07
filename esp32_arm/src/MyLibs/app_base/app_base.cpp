#include "app_base.h"
#include "MyLibs/mqtt/mqtt_subscriber_manager.h"

#define APP_COMMAND_PREFIX  "app:"
#define APP_COMMAND_PREFIX_SIZE 4


void AppBase::SpinOnce(){
    __deal_feedback();
    if (! _text_message_queue.BufferIsEmpty()){
        __dispach_tail_message();
    }
    if (__is_lua_running_file){
        __Lua_RunLine_ofFile();
    }
}

void AppBase::__dispach_tail_message(){
    TextMessageLine* line = _text_message_queue.GetWithdrawTailElement(false);
    switch (line->GetCategory()){
        case TextMessageLine::Enum_Category::GCODE:
            if (__robot == nullptr){
                Logger::Error("Try to feed gcode to robot,  but robot is nullptr");
            }else{
                if (__robot->GetGcodeQueue()->GetFreeBuffersCount() >3 ){
                    //TODO:  from text_message_queue to gcode_queue.
                }
            }
            break;
        case TextMessageLine::Enum_Category::LUA:
            if (__lua == nullptr){
                Logger::Error("Try to feed gcode to LUA,  but LUA is nullptr");
            }else{
                line->RemovePrefix();
                Logger::Print("get lua text", line->GetChars());
                __lua->Lua_dostring(line->GetChars());
                _text_message_queue.WithdrawTailElement();
            }
        default:
            break;
    }

}

//****************************************************************************************
//              MQTT
//
void AppBase::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    //format string ender.
    char* p = (char*)(payload);
    p[payload_len] = 0x00;  
    Logger::Debug("AppBase::onGot_MqttMessage");
    Logger::Print("payload", payload);
    Logger::Print("payload_len", payload_len);
    // const char* IS_APP_COMMAND = APP_COMMAND_PREFIX;

    __have_done_feedback = false;
    // TextMessageLine command_text;
    __app_command.CopyFrom(payload,payload_len);
    if (__app_command.IsEqualTo("app:led")){
        // This is a thread in mqtt-on-received callbaking.    so watchdog will be fired if long time without return.
        // this->ExecuteAppCommand(&__app_command);
        return;
    }
    __app_command.CopyFrom("none");
    int free_buffer_count = _text_message_queue.GetFreeBuffersCount();
    if (free_buffer_count <=1 ){
        Logger::Error("AppBase::onGot_MqttMessage()  buffer is full");
        return;
    }
    // copy to local _text_message_queue
    this->_text_message_queue.AppendTextMessageLine(payload, payload_len);
}

void AppBase::Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic){
    int topic_len;
    for(topic_len=0; topic_len<REPRAP_GCODE_MAX_SIZE; topic_len++){
        // Logger::Print("mqtt_topic[topic_len]", mqtt_topic[topic_len]);
        if (mqtt_topic[topic_len] == 0x00){
            break;
        }
        __mqtt_topic_feedback[topic_len] = mqtt_topic[topic_len];
    }
    __mqtt_topic_feedback[topic_len] = '/';
    __mqtt_topic_feedback[topic_len + 1] = 'f';
    __mqtt_topic_feedback[topic_len + 2] = 'b';
    __mqtt_topic_feedback[topic_len + 3] = 0x00;   //ender

    gs_MqttSubscriberManager::Instance().AddSubscriber(mqtt_topic, this);
    
    // Logger::Print("AppBase::Link_Mqtt_to_TextMessageQueue()   mqtt_topic", mqtt_topic);
    // Logger::Print("AppBase::Link_Mqtt_to_TextMessageQueue()  this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
}

void AppBase::__deal_feedback(){
    if (__have_done_feedback)
        return;
    if (__app_command.IsPrefix("app:")){
        g_mqttClient.publish(this->__mqtt_topic_feedback, 2, true, __app_command.GetChars());
        this->ExecuteAppCommand(&__app_command);
    }
    if (_text_message_queue.GetFreeBuffersCount() == 0)
        return;

    // Logger::Debug("AppBase::SpinOnce()   send_feedback, started");
    TextMessageLine* command_text = _text_message_queue.GetDepositHeadElement();

    // Logger::Print("send_feedback, this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
    // Logger::Print("send_feedback, payload", command_text->GetChars);
    // command_text->PrintFlat("verify origin");
    g_mqttClient.publish(this->__mqtt_topic_feedback, 2, true, command_text->GetChars());
    __have_done_feedback = true;
    // Logger::Print("send_feedback, finished.", 99);
}


//*****************************************************************************************
//              LUA
//
void AppBase::__Lua_RunLine_ofFile(){
    // Logger::Debug("AppBase::__Lua_RunLine_ofFile()");
	if (__lua_file.available()) {
		String line = __lua_file.readStringUntil('\n');
		Logger::Info(line.c_str());
		String result = __lua->Lua_dostring(&line);
		Serial.println(result);
	}else{
		__lua_file.close();
		__is_lua_running_file = false;   //???
		Logger::Info("LuaWrapperBase::SpinOnce()  Run lua file ending report");
	}

}

void AppBase::Link_lua_from_File(LuaWrapperBase* lua, const char* filename){
    __lua_filename.CopyFrom(filename);
    __lua = lua;
    __lua_file = SPIFFS.open(filename, FILE_READ);
	Logger::Debug("LuaWrapperBase::Begin()");
	Logger::Print("filename", filename);
    __lua->Begin();
    __is_lua_running_file = true;
}

void AppBase::Link_lua_from_Mqtt(LuaWrapperBase* lua, const char* mqtt_topic){
    __lua = lua;
    this->Link_Mqtt_to_TextMessageQueue(mqtt_topic);
}


