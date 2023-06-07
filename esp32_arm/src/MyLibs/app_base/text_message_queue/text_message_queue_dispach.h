#pragma once
#include "../lua_wrapper/lua_wrapper_base.h"
// #include "../app_base.h"
#include "Robot/robot_base.h"
#include "text_message_queue.h"

class MessageQueueDispacher{
    public:
        void SpinOnce();


    private:
        LuaWrapperBase* __lua;
        // AppBase* __app;
        RobotBase* __robot;
        CommandQueue* __text_message_queue;
};