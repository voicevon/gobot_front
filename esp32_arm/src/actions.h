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
        bit[4] == 0: skip home_x
        bit[4] == 1: do home_x
        bit[5] == 0: skip home_y
        bit[5] == 1: do home_y
        bit[6] == 0: Don't draw stone from house
        bit[6] == 1: draw stone from house_id
        bit[7] == 0: Don't move stone 
        bit[7] == 1: Move_stone_From house_id
*/
  typedef union {
    struct {
      uint8_t action_code;
      // uint8_t pickup_type;   // 0=skip, 1=do it.
      int16_t pickup_x;
      int16_t pickup_y;
      // uint8_t place_type;    // 0=skip, 1=lower release, 2=higher release
      int16_t place_x;
      int16_t place_y;
      // uint8_t do_park;       //0= skip, 1=do it.
      int16_t park_x;
      int16_t park_y;
    }Arm;
    struct{
      uint8_t action_code;
      uint16_t from_start_house_id;  //From or start house id
    }House;
    byte bytes[13];
  }BodyAction;

  // struct  HouseAction{
  //   unsigned char action_code;  // 2=forward   4= backward
  //   unsigned char from_to_house_id;  // from_house_id for forwarding .  to_house_id for backwarding.

  // };
  

