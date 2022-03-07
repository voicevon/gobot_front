
#include "Robot/gcode_queue.h"
#include "vehical_twin_wheel_hw.h"


class VehicalTwinWheel{

    public:
        VehicalTwinWheel(){};
        TwinWheelHardware objTwinWheel_hardware;        
        void Init();
        GcodeQueue* GetCommandQueue(){return this->__commandQueue;};

    private:
        GcodeQueue* __commandQueue;

};

