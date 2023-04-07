#include "Robot/gcode_runner/g28_runner.h"
#include "remote_home_position.h"


class Twh_LoopPorter_G28_Runner: public G28_Runner{
    public:
        void Init(CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position) override;

    private:
        void _InitHomePosition(char axis_name, float setting_position) override;
        void _SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
        void _SetHomedPosition(PositionTrigger* firer) override;
        RemoteHomePosition __remote_home_position;
};