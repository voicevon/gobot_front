#include "Robot/gcode_runner/g28_runner.h"

class Spring_G28_Runner: public G28_Runner{
    public:

    private:
        void SetMoveBlock_ToHome(EnumAxis axis, MoveBlock* mb) override;

};