#pragma once

#include "MyLibs/mqtt/mqtt_subscriber_base.h"
#include "text_message_queue/text_message_queue.h"
#include "Robot/robot_base.h"
#include "lua_wrapper/lua_wrapper_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#include "SPIFFS.h"

// #define TEXT_MESSAGE_QUEUE_SIZE 20

class AppBase: public MqttSubscriberBase{
    public:
        // void Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic, TextMessageQueue* text_command_queue);
        // void Init();
        void Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic);
        void Link_Robot(RobotBase* robot){__robot=robot;};
        void Link_lua_from_File(LuaWrapperBase* lua, const char* filename);
        void Link_lua_from_Mqtt(LuaWrapperBase* lua, const char* mqtt_topic);
        

        virtual void ExecuteAppCommand(TextMessageLine* gcode_text){}; 

        void SpinOnce(); 
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;


    protected:
        TextMessageQueue* _text_message_queue;
        // TextMessageLine __all_text_message[TEXT_MESSAGE_QUEUE_SIZE];

    private:
        void __Lua_RunLine_ofFile(); 
        void __deal_feedback();
        void __dispach_tail_message();
        int test_id = 1234;
        bool __have_done_feedback = true;
        char __mqtt_topic_feedback[40];
        TextMessageLine __app_command;
        RobotBase* __robot = nullptr;

        // Lua and Lua file
        LuaWrapperBase* __lua = nullptr;
        FsFilename __lua_filename;
		File __lua_file;
        bool __is_lua_running_file = false;

};