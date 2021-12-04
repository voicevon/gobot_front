
#include "Gcode.h"
#include "RobotBase.h"

class GcodeDispacher{
    public:
        GcodeDispacher();
        void LinkRobot(RobotBase* mybot);
        void SpinOnce();
        bool AppendGcode(Gcode* gcode);
        
    private:
        Gcode* _buffer[5];
        int head;
        int tail;
        RobotBase* _myBot;
};