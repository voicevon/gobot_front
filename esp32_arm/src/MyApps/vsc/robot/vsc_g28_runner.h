#include "Robot/gcode_runner/g28_runner.h"
#include "Robot/axis_homer/home_trigger_array.h"

class Vsc_G28_Runner: public G28_Runner{
    public:
        void Init(CncMoverBase* mover) override;

    private:
        void SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;

};