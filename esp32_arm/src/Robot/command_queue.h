#pragma once

#include "Gcode.h"
#include "RobotBase.h"


#define MAX_BYTES_PER_COMMAND 20
#define COMMANDS_COUNT_IN_QUEUE 22

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
        // This is a buffer , can contain 20 bytes per command, and 22 commands in the queue.
        char gCodeCommands[MAX_BYTES_PER_COMMAND * COMMANDS_COUNT_IN_QUEUE];  // or [20][5]?
        int head;
        int tail;
        RobotBase* _myBot;
};