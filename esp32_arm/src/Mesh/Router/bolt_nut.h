#pragma once


#include "stdint.h"

class Neibour{
    public:
        uint8_t net_id;   // In case of:  multi mesh-network work in same area. ??
        uint8_t id;   // 0 = empty 
        uint8_t hop;
        uint8_t mac_addr[6];
};


class Package{
    public:
        uint8_t my_hop;
        char net_id[6];
        char to_mac_addr[6];
        uint8_t package_size;
        char payload[99];

        void PrintOut(const char* title);
};

class AdhocHelper{
    public:
        static bool IsSameMacAddr();
};