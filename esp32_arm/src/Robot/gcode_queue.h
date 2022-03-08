#pragma once

#include "Gcode.h"
#include "RobotBase.h"
#include "MyLibs/message_queue.h"

class GcodeQueue: public MessageQueue{
    public:
        GcodeQueue();
        void LinkRobot(RobotBase* mybot);
        void LinkMessageConsumer(RobotBase* mybot);
        // bool AppendGcode(Gcode* gcode);
        bool AppendGcodeCommand(String command);
        // void SpinOnce() override;

    private:
        RobotBase* _myBot;
        
};