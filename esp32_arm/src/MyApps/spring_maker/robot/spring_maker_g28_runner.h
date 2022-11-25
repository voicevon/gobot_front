#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/spring_maker/board/board.h"

class SpringMaker_G28_Runner: public G28_Runner{
    public:
        void Init(CncMover* mover) override;

    private:
        void SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;

};