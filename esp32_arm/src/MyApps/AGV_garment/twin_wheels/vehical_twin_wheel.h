
#include "Robot/command_queue.h"
#include "vehical_twin_wheel_hw.h"


class VehicalTwinWheel{

    public:
        VehicalTwinWheel(){};
        TwinWheelHardware objTwinWheel_hardware;        
        void Init();
        CommandQueue* GetCommandQueue(){return this->__commandQueue;};

    private:
        CommandQueue* __commandQueue;

};

