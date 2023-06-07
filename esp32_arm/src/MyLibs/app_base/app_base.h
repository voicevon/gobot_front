#pragma once

#include "MyLibs/mqtt/mqtt_subscriber_base.h"
#include "text_message_queue/text_message_queue.h"
#include "Robot/robot_base.h"
#include "lua_wrapper/lua_wrapper_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#include "SPIFFS.h"



class AppBase: public MqttSubscriberBase{
    public:
        // void Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic, TextMessageQueue* text_command_queue);
        void Link_Mqtt_to_TextMessageQueue(const char* mqtt_topic);
        void Link_Robot(RobotBase* robot){__robot=robot;};
        void Link_lua_from_File(LuaWrapperBase* lua, const char* filename);
        void Link_lua_from_Mqtt(LuaWrapperBase* lua, const char* mqtt_topic);
        

        virtual void ExecuteAppCommand(TextMessageLine* gcode_text){}; 

        void SpinOnce(); 
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override ;


    protected:
        TextMessageQueue* _text_message_queue;
        void Lua_SpinOnce();

    private:
        void __deal_feedback();
        void __dispach_head_message();
        int test_id = 1234;
        bool __have_done_feedback = true;
        char __mqtt_topic_feedback[40];
        TextMessageLine __app_command;
        RobotBase* __robot=nullptr;

        // Lua and Lua file
        LuaWrapperBase* __lua=nullptr;
        FsFilename __lua_filename;
		File __lua_file;
        

};