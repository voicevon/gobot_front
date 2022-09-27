#pragma opnce


#include "Robot/robot_base.h"
#include "MyApps/vsc/board/board_vsc.h"
#include "MyApps/vsc/cnc/vsc_arm_solution.h"

class VscRobot: public RobotBase{
    public:
        void Init(Vsc_Board* board, Vsc_ArmSoution *arm_solution);

    private:
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};

        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        PidControllers __all_pids = PidControllers(1);
        Planner __planner;
        Vsc_CncMover __mover;
        MoveBlock __all_move_blocks[88]; 
        MoveBlockQueue __queue_move_block;
        // MoveBlock* __all_move_blocks_pointer[88];


};