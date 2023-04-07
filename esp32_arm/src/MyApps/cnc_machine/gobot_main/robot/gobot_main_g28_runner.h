#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/cnc_machine/gobot_main/board/board.h"

class GobotMain_G28_Runner: public G28_Runner{
    public:
        void Init(CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position) override;

    private:
        void _SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
        void _SetHomedPosition(PositionTrigger* firer) override;

};