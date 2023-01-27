#pragma once


#include "stdint.h"

class Neibour{
    public:
        uint8_t id;   // 0 = empty 
        uint8_t hop;
        char mac_addr[6];
};


class Package{
    public:
        uint8_t my_hop;
        char source_net_id[6];
        char to_mac_addr[6];
        uint8_t package_size;
        char payload[99];
};