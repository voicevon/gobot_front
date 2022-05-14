#include "obstacle_sensor_hcsr04.h"

ObstacleSensor_Hcsr04::ObstacleSensor_Hcsr04(const uint8_t HCSR04_PIN_TRIG, const uint8_t HCSR04_PIN_ECHO){
	this->hcsr04 = new UltraSonicDistanceSensor(HCSR04_PIN_TRIG, HCSR04_PIN_ECHO);
}

bool ObstacleSensor_Hcsr04::DetectObstacle(){
    this->_distance_to_obstacle =  this->hcsr04->measureDistanceCm(); 
    float distance_to_obstacle = -1;   // For test.
    bool found_obstacle = false;
    if (distance_to_obstacle >0 && distance_to_obstacle <50) 
        found_obstacle = true;
    return found_obstacle;
}