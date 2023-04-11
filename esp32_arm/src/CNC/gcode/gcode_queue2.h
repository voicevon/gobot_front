#include "MyLibs/basic/queue/queue_base.h"
#include "Gcode.h"

class GcodeQueue2: public QueueBase<Gcode>{
    public:
        GcodeQueue2(){};
        
        bool AppendGcodeCommand(String command){
            // return this->AppendMessage(command);
            Gcode* gcode =  this->_GetRoom();
            gcode->command = command;
            this->Deposit();
        };

        bool AppendGcodeCommand(const char* payload, int length){
            // return this->AppendMessage(payload, length);
        };

    private:
        
};