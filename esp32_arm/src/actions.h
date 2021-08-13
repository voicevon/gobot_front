#pragma once

#include <Arduino.h>
/*
        action
        bit[0] == 0: do nothing. or last job is finished by esp32 
                        Setting by head
               == 1: executing task
                        Setting by esp_arm

        bit[1] == 0: skip pickup
        bit[1] == 1: move to pickup point.
        bit[2] == 0: skip place down.
        bit[2] == 1: move to place point
        bit[3] == 0: skip parking.
        bit[3] == 1: move to parking point.
        bit[4] == 0: skip arm home
        bit[4] == 1: Arm home
            Value[4]: Arm_Alpha
            Value[5]: Arm_Beta
        bit[5] == 0: skip house home
        bit[5] == 1: do house home
            Value[8]: House_Alpha
            Value[9]: House_Beta
        bit[6] == 0: Don't draw stone from house
        bit[6] == 1: draw stone from house_id
        bit[7] == 0: Don't move stone 
        bit[7] == 1: Move_stone_From house_id
*/
  typedef union {
    struct {
      uint8_t action_code;
      uint8_t alignment_occupied_by_compiler;   //Assume without this var, the size of the union is 14, not 13!!!
      int16_t pickup_x;
      int16_t pickup_y;
      int16_t place_x;
      int16_t place_y;
      int16_t park_x;
      int16_t park_y;
    }Arm;
    struct{
      uint8_t action_code;
      uint16_t from_start_house_id;  //From or start house id
    }House;
    byte bytes[14];
  }RobotAction;

  // struct  HouseAction{
  //   unsigned char action_code;  // 2=forward   4= backward
  //   unsigned char from_to_house_id;  // from_house_id for forwarding .  to_house_id for backwarding.

  // };
