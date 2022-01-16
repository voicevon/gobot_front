#pragma once

#include "Gcode.h"
#include "RobotBase.h"


#define MAX_BYTES_PER_COMMAND 20
#define COMMANDS_COUNT_IN_QUEUE 5

class CommandQueue{
    public:
        CommandQueue();
        void LinkRobot(RobotBase* mybot);
        void SpinOnce();
        // bool AppendGcode(Gcode* gcode);
        bool AppendGcodeCommand(String command);
        bool BufferIsEmpty();
        int GetFreeBuffersCount();
    private:
        // Gcode* _buffer[5];
        char gCodeCommands[MAX_BYTES_PER_COMMAND * COMMANDS_COUNT_IN_QUEUE];  // or [20][5]?
        int head;
        int tail;
        RobotBase* _myBot;
};