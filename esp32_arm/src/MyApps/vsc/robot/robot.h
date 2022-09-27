#pragma opnce


#include "Robot/robot_base.h"
#include "../board/board_vsc.h"
#include "../cnc/solution.h"

class VscRobot: public RobotBase{
    public:
        void Init(Vsc_Board* board, Vsc_CncSoution *arm_solution);

    private:
        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        PidControllers __all_pids = PidControllers(1);
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};
        // virtual EnumAxis ConvertToEnum(char axis_name);

};