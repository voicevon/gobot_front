#pragma once

#include <Arduino.h>

  typedef union {
    struct {
      uint8_t action_code;
      uint8_t pickup_type;   // 0=skip, 1=do it.
      int32_t pickup_x;
      int32_t pickup_y;
      uint8_t place_type;    // 0=skip, 1=lower release, 2=higher release
      int32_t place_x;
      int32_t place_y;
      uint8_t do_park;       //0= skip, 1=do it.
      int32_t park_x;
      int32_t park_y;
    }Attr;
    byte bytes[52];
  }ArmAction;

  struct  HouseAction{
    unsigned char action_code;  // 2=forward   4= backward
    unsigned char from_to_house_id;  // from_house_id for forwarding .  to_house_id for backwarding.

  };
  

