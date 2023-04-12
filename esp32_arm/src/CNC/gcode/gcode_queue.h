#pragma once

#include "MyLibs/basic/queue/queue_base.h"
#include "Gcode.h"

class GcodeQueue: public QueueBase<Gcode>{
    public:
        GcodeQueue(){};
        bool AppendGcodeCommand(String command){
            // return this->AppendMessage(command);
            Gcode* gcode =  this->_GetRoom();
            gcode->command = (char*) (command.c_str());
            this->Deposit();
        };

        bool AppendGcodeCommand(const char* payload, int length){
            // return this->AppendMessage(payload, length);
            Gcode* gcode = this->_GetRoom();
            char* destination = gcode->command;
            for (int i=0; i<length; i++){
                *(destination + i) = payload[i];
            }
        };

    private:
        
};