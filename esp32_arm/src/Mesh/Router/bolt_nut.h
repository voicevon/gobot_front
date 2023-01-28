#pragma once


#include "stdint.h"

class Neibour{
    public:
        uint8_t net_id;   // In case of:  multi mesh-network work in same area. ??
        uint8_t id;   // 0 = empty 
        uint8_t hop;
        uint8_t mac_addr[6];
};


class AdhocPackage{
    public:
        uint8_t my_hop;
        uint8_t network_id;
        uint8_t to_mac_addr[6];
        uint8_t package_size;
        uint8_t payload[99];

        void PrintOut(const char* title);
};

class AdhocHelper{
    public:
        static bool IsSameMacAddr(uint8_t* addr_a, uint8_t* addr_b);
        static void CopyMacAddr(uint8_t* source, uint8_t* target);
};