#include "CNC/cnc_single_axis/cnc_single_axis_config.h"

class SpringMakerHardwareConfig: public CncSingleAxis_Config{
    public:
        int32_t GetStepsPerUnit(char axis_name) override;
        void Init() override;
        void PrintOut() override;
        
    private:

};