
#include "Gcode.h"
#include "RobotBase.h"

class GcodeDispacher{
    public:
        GcodeDispacher();
        void LinkRobot(RobotBase* mybot);
        void SpinOnce();
        // bool AppendGcode(Gcode* gcode);
        bool AppendGcodeCommand(String command);
        
    private:
        // Gcode* _buffer[5];
        char gCodeCommands[20*5];  // or [20][5]?
        int head;
        int tail;
        RobotBase* _myBot;
};