# pragma once
#include "bolt_nut.h"
#include "WiFi.h"
#include <esp_now.h>

#define ROUTER_TABLE_ROWS  16

class AdhocRouter{
    public:
        void onReceived(const uint8_t * mac, const uint8_t *incomingData, int len);
        void Init(bool i_am_net_gate);
        void SpinOnce();

        void Send(AdhocPackage* pkg);
    protected:


    private:
        Neibour __my_neibours[ROUTER_TABLE_ROWS];
        Neibour* __search_neibour(uint8_t * mac_addr);
        bool __is_same_mac_addr(uint8_t* addr_a, uint8_t* addr_b);
        Neibour* __find_blank_neibour();

        void __send_out(AdhocPackage* package);
        void __Broadcast_Iam_Orphan();
        

        uint8_t __my_hop;
        uint32_t __time_count_up;
        AdhocPackage __orphan_package;
        void __onPackage_received(const uint8_t * mac, AdhocPackage* incoming_package);




};