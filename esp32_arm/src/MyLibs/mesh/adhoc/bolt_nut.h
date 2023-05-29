#pragma once


#include "stdint.h"
#define NODE_ID_MESH_GATE 101
class Neibour{
    public:
        // uint8_t net_id;   //TODO:   In case of:  multi mesh-network work in same area. ??
        uint8_t node_id;   // 0 = empty 
        uint8_t hop;
        uint8_t mac_addr[6];
        int leader_ship;  // default value == 90.
        void PrintOut(const char* title);
};

class MeshBranch{
    uint8_t my_child_node_id;
    uint8_t my_child_family[20];  // How to become dynamic size?
};


class AdhocPackage{
    public:
        uint8_t network_id;     // Currently only one network in the range.
        // 1: app_data to net-gate   2: app_data_to router or sensor.     11: to router or sensor, but failed and backwording to source_node.
        // 21:  uploading my_route_table  appended  22: uploading my_route_table, please  removed this node, include myself.    
        uint8_t package_type;
        uint8_t to_mac_addr[6];     // current hop target.
        uint8_t sender_node_id;
        uint8_t sender_hop;        // EspNow sender, Point to Point sender.
        uint8_t source_node_id;
        uint8_t destination_node_id;
        uint8_t app_payload_size;
        uint8_t app_payload[99];   // Max pack size of esp-now is ??

        // void PrintOut(const char* title, Neibour* leader);
        void PrintOut(const char* title);
};

class AdhocHelper{
    public:
        static bool IsSameMacAddr(uint8_t* addr_a, uint8_t* addr_b);
        static void CopyMacAddr(uint8_t* source, uint8_t* target);
};