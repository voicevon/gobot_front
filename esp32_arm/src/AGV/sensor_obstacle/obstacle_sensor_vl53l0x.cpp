#include "obstacle_sensor_vl53l0x.h"


// ObstacleSensor_VL53l0x::ObstacleSensor_VL53l0x(TwoWire* i2c_bus, uint8_t i2c_address){
//     Serial.println("Adafruit VL53L0X test");
//     this->__vl53lox=new Adafruit_VL53L0X();
//     if(!this->__vl53lox->begin(i2c_address, true, i2c_bus)){
//         Serial.println(F("Failed to boot VL53L0X"));
//         while(1);
//     }
//     Serial.println(F("ObstacleSensor-VL53L0X is started.\n\n")); 
// }
ObstacleSensor_VL53l0x::ObstacleSensor_VL53l0x(Adafruit_VL53L0X* vl53l0x){
    this->__vl53lox = vl53l0x;
}

bool ObstacleSensor_VL53l0x::DetectObstacle(){
    VL53L0X_RangingMeasurementData_t measure;
    Serial.print("[Warn] ObstacleSensor_VL53l0x::DetectObstacle() ");

    this->__vl53lox->rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print("[Info] ObstacleSensor_VL53l0x::DetectObstacle()  Measured Distance (mm): ");
        Serial.println(measure.RangeMilliMeter);
        return true;  // Found obstacle
    } else {
        Serial.println(" out of range ");
        return false;   // Not found obstacle
    }
}
