#include "app_base.h"
#include "MyLibs/mqtt/mqtt_subscriber_manager.h"
#include "Mylibs/utility/webserver_starter/webserver_starter.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"  //g_mqtt_client


void AppBase::Init(){
    __text_message_category = TextMessageLine::Enum_Category::GCODE;
}

void AppBase::SpinOnce(){
	// Logger::Debug("333333333");
    __deal_feedback();
    // _text_message_queue.PrintOut("ttttttttttttttttttttt");
    if (! _text_message_queue.BufferIsEmpty()){
        __dispach_tail_message();
    }

    if (__lua != nullptr){
        // Serial.print("a");
        __lua->SpinOnce();
    }

}

void AppBase::__dispach_tail_message(){
	Logger::Debug("555555555555555");
    TextMessageLine* tail_text = _text_message_queue.GetWithdrawTailElement(false);
	Logger::Debug("666666666666");
    tail_text->SetCatogory(__text_message_category);
	Logger::Debug("7777777777777");
    if (tail_text->IsCategoryUpdated()){
        __text_message_category = tail_text->GetCategory();
    }
	Logger::Debug("888888888888888");
    switch (tail_text->GetCategory()){
        case TextMessageLine::Enum_Category::FILE:
            __file_writter.FeedText(tail_text);
            break;
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
            // if (__lua == nullptr){
            //     Logger::Error("Try to feed gcode to LUA,  but LUA is nullptr");
            // }else{
                tail_text->RemovePrefix(':');
                Logger::Print("get lua text", tail_text->c_str());
                __lua->Lua_dostring(tail_text->c_str());
                _text_message_queue.WithdrawTailElement();
            // }
            break;
        default:
            Logger::Error("AppBase::__dispach_tail_message()   out of managerment.");
            break;
        }

}

void AppBase::StartWebServer(ApWebserver_DictionBase* diction){
	WebServerStarter::Begin(diction);
}

//****************************************************************************************
//              MQTT
//
void AppBase::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    if (payload_len > __head_text_message.GetBufferSize()){
        String pp = String(payload);
        Logger::Error("AppBase::onGot_MqttMessage() oversize");
        return;
    }
    //format string ender.
    char* p = (char*)(payload);
    p[payload_len] = 0x00;  
    Logger::Debug("AppBase::onGot_MqttMessage");
    Logger::Print("payload", payload);
    Logger::Print("payload_len", payload_len);
    // const char* IS_APP_COMMAND = APP_COMMAND_PREFIX;
    
    __have_done_feedback = false;
    // TextMessageLine command_text;
    __head_text_message.CopyFrom(payload,payload_len);
    if (__head_text_message.IsEqualTo("app:led")){
        // This is a thread in mqtt-on-received callbaking.    so watchdog will be fired if long time without return.
        // this->ExecuteAppCommand(&__head_text_message);
        return;
    }
    __head_text_message.CopyFrom("none");
    int free_buffer_count = _text_message_queue.GetFreeBuffersCount();
    if (free_buffer_count <=1 ){
        Logger::Error("AppBase::onGot_MqttMessage()  buffer is full");
        return;
    }
    // copy to local _text_message_queue
    this->_text_message_queue.AppendTextMessageLine(payload, payload_len);
}

void AppBase::Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic){
    // int topic_len;
    // for(topic_len=0; topic_len<REPRAP_GCODE_MAX_SIZE; topic_len++){
    //     // Logger::Print("mqtt_topic[topic_len]", mqtt_topic[topic_len]);
    //     if (mqtt_topic[topic_len] == 0x00){
    //         break;
    //     }
    //     // __mqtt_topic_feedback[topic_len] = mqtt_topic[topic_len];
    // }
    // __mqtt_topic_feedback[topic_len] = '/';
    // __mqtt_topic_feedback[topic_len + 1] = 'f';
    // __mqtt_topic_feedback[topic_len + 2] = 'b';
    // __mqtt_topic_feedback[topic_len + 3] = 0x00;   //ender
    __mqtt_topic_feedback.CopyFrom(mqtt_topic);
    __mqtt_topic_feedback.Concat("/fb");
    gs_MqttSubscriberManager::Instance().AddSubscriber(mqtt_topic, this);
}

void AppBase::__deal_feedback(){
    if (__have_done_feedback)
        return;
    // if (__head_text_message.IsPrefix("app:")){
    //     g_mqttClient.publish(this->__mqtt_topic_feedback.c_str(), 2, true, __head_text_message.c_str());
    //     this->ExecuteAppCommand(&__head_text_message);
    // }
    if (_text_message_queue.GetFreeBuffersCount() == 0)
        return;

    // Logger::Debug("AppBase::SpinOnce()   send_feedback, started");
    TextMessageLine* command_text = _text_message_queue.GetDepositHeadElement();

    // Logger::Print("send_feedback, this->__mqtt_topic_feedback", this->__mqtt_topic_feedback);
    // Logger::Print("send_feedback, payload", command_text->GetChars);
    // command_text->PrintFlat("verify origin");
    g_mqttClient.publish(this->__mqtt_topic_feedback.c_str(), 2, true, command_text->c_str());
    __have_done_feedback = true;
    // Logger::Print("send_feedback, finished.", 99);
}


//*****************************************************************************************
//              LUA
//

void AppBase::Link_Lua(LuaBase* lua){
    __lua = lua;
}

void AppBase::Lua_DoFile(LuaBase* lua, const char* filename){
	Logger::Debug("LuaBase::Begin()");
	Logger::Print("filename", filename);
    __lua = lua;
    __lua_file = SPIFFS.open(filename, FILE_READ);
    __lua->Begin();
    String content = __lua_file.readString();
    __lua->Lua_dostring(content.c_str());
    String result = __lua->Lua_dostring("setup()");
}




