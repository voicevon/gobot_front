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
        // uint8_t network_id;     // Currently only one network in the range.
        uint8_t sender_hop;
        uint8_t to_mac_addr[6];     // current hop target.
        uint8_t app_source_node_id;
        uint8_t app_payload_size;
        uint8_t app_payload[99];   // Max pack size of esp-now is ??

        void PrintOut(const char* title);
};

class AdhocHelper{
    public:
        static bool IsSameMacAddr(uint8_t* addr_a, uint8_t* addr_b);
        static void CopyMacAddr(uint8_t* source, uint8_t* target);
};