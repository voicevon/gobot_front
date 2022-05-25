#pragma once

// 2 to 15 cm GP2Y0A51SK0F
// 4 to 30 cm GP2Y0A41SK0F / GP2Y0AF30 series
// 10 to 80 cm GP2Y0A21YK0F
// 10 to 150 cm GP2Y0A60SZLF
// 20 to 150 cm GP2Y0A02YK0F
// 100 to 550 cm GP2Y0A710K0F

#pragma once

#include "obstacle_sensor_base.h"
#include "Robot/Sensor/sharp_ir_distance_adc.h"
#include "stdint.h"

class ObstacleSensor_SharpIr:public ObstacleSensorBase{
    public:
        ObstacleSensor_SharpIr(uint8_t pin_adc);

    protected:
        bool DetectObstacle() override;

    private:
        SharpIrAdc* __sharp_ir;
};