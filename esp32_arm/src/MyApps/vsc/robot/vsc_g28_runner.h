#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/vsc/board/board_vsc.h"
#include "Robot/axis_homer/home_trigger_array.h"
// #include "Robot/axis_homer/homer_diction.h"
#define HOMERS_COUNT 1

class Vsc_G28_Runner: public G28_Runner{
    public:
        void Init(MoverBase* mover) override;

    private:
        void SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;

};