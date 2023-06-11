#pragma once

#include "MyLibs/mqtt/mqtt_subscriber_base.h"
#include "text_message_queue/text_message_queue.h"
#include "Robot/robot_base.h"
#include "lua_base/lua_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#include "SPIFFS.h"
#include "Mylibs/utility/webserver_starter/web_configurator_diction.h"
#include "file_writter/file_writter.h"


// #define TEXT_MESSAGE_QUEUE_SIZE 20

class AppBase: public MqttSubscriberBase{
    public:
        // AppBase():MqttSubscriberBase(){};
        // virtual ~AppBase(){};
        void Init();
        void StartWebServer(ApWebserver_DictionBase* diction);
        void Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic);
        void Link_Robot(RobotBase* robot){__robot=robot;};
        void Lua_DoFile(LuaWrapperBase* lua, const char* filename);
        // void Link_lua_from_Mqtt(LuaWrapperBase* lua, const char* mqtt_topic);

        // virtual void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;

        void SpinOnce(); 


    protected:

    private:
        TextMessageQueue _text_message_queue;
        void __deal_feedback();
        void __dispach_tail_message();
        int test_id = 1234;

        MqttTopic __mqtt_topic_feedback;
        bool __have_done_feedback = true;
        TextMessageLine __head_text_message;
        TextMessageLine::Enum_Category __text_message_category ; // = TextMessageLine::Enum_Category::GCODE;

        RobotBase* __robot = nullptr;
        FileWritter __file_writter;

        // Lua and Lua file
        LuaWrapperBase* __lua = nullptr;
		File __lua_file;
        bool __is_lua_running_file = false;

};