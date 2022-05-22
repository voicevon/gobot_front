#include "obstacle_sensor_vl53l0x.h"


ObstacleSensor_VL53l0x::ObstacleSensor_VL53l0x(TwoWire* i2c_bus, uint8_t i2c_address){
    Serial.println("Adafruit VL53L0X test");
    if (!this->__vl53lox->begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
        while(1);
    }
    // power 
    Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

bool ObstacleSensor_VL53l0x::DetectObstacle(){
    VL53L0X_RangingMeasurementData_t measure;
        Serial.print("Reading a measurement... ");
    this->__vl53lox->rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print("Distance (mm): ");
        Serial.println(measure.RangeMilliMeter);
        return true;  // Found obstacle
    } else {
        Serial.println(" out of range ");
        return false;   // Not found obstacle
    }
}
