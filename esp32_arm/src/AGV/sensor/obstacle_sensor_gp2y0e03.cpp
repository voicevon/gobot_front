

// https://forum.arduino.cc/t/how-to-change-i2c-address-on-gp2y0e03-sensor-by-e-fuse-solved/972294

// https://github.com/martinpalsson/GP2Y0E02B_E-Fuse_Burner


#include "obstacle_sensor_gp2y0e03.h"

ObstacleSensor_Gp2y0e03::ObstacleSensor_Gp2y0e03(TwoWire* i2c_bus, uint8_t i2c_address){

}

bool ObstacleSensor_Gp2y0e03::DetectObstacle(){
    return false;
}