# include "HouseMap.h"
#include <math.h>

void HouseMap::__init(){
    this->neck.y = 0;
    this->neck.x = 60.0;
    this->head.y = 0;
    this->head.x = 148.93;

    this->rooms[0].x = -119.51;
    this->rooms[0].y = 126.38;
    this->rooms[1].x = -111.9;
    this->rooms[1].y = 95.34;
    this->rooms[2].x = -98.18;
    this->rooms[2].y = 61.4;
    this->rooms[3].x = -78.79;
    this->rooms[3].y = 25.59;

    this->rooms[4].x = this->rooms[3].x;
    this->rooms[4].y = - this->rooms[3].y;
    this->rooms[5].x = this->rooms[2].x;
    this->rooms[5].y = - this->rooms[2].y;
    this->rooms[6].x = this->rooms[1].x;
    this->rooms[6].y = - this->rooms[1].y;
    this->rooms[7].x = this->rooms[0].x;
    this->rooms[7].y = - this->rooms[0].y;

    this->doors[0].x = -49.46;
    this->doors[0].y = 59.07;
    this->doors[1].x = -81.22;
    this->doors[1].y = 56.12;
    this->doors[2].x = -104.11;
    this->doors[2].y = 40.67;
    this->doors[3].x = -117.25;
    this->doors[3].y = 17.95;
    this->doors[4].x = this->doors[3].x;
    this->doors[4].y = - this->doors[3].y;
    this->doors[5].x = this->doors[2].x;
    this->doors[5].y = - this->doors[2].y;
    this->doors[6].x = this->doors[1].x;
    this->doors[6].y = - this->doors[1].y;
    this->doors[7].x = this->doors[0].x;
    this->doors[7].y = - this->doors[0].y;
}
void HouseMap::setup(int segments){
    __segments = segments;
    __init();

    distance_head_to_neck = head.x - neck.x ;

    for(int i=0; i<8; i++){
        float dx = doors[i].x - rooms[i].x;
        float dy = doors[i].y - rooms[i].y;
        distance_room_to_door[i] = sqrtf(dx * dx + dy * dy);
    }

}

float HouseMap::get_dx_distance(int house_id){
    return (doors[house_id].x - rooms[house_id].x) / __segments;
}
float HouseMap::get_dy_distance(int house_id){
    float distance = abs(doors[house_id].y - rooms[house_id].y);
    return distance / __segments;
}
