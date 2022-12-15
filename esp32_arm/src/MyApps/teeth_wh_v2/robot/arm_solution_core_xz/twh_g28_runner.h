#include "Robot/gcode_runner/g28_runner.h"

class Twh2_G28_Runner_CoreXz: public G28_Runner{
    public:
        void Init(CncMover* mover, ArmSolutionBase* arm_solution) override;

    private:
        void SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
        void SetHomedPosition(PositionTrigger* firer) override;

};