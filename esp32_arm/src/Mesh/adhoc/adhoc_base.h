#pragma once
#include "bolt_nut.h"
#include "WiFi.h"
#include <esp_now.h>
#include "esp_wifi.h"

#define ROUTER_TABLE_ROWS  16


class AdhocBase{
    public:
        // return true:  incomingData is from leader
        virtual bool onReceived(const uint8_t * sender_mac, const uint8_t *incomingData, int len);
        virtual void Init(uint8_t my_node_id);
        
        void Broadcast_Orphan_count_down();

    protected:
        void _Init_EspNow();
        uint8_t _my_mac_addr[6];
        uint8_t _my_node_id;
        uint8_t _my_hop;

        uint32_t _time_count_up;
        AdhocPackage _orphan_package;
        void _Send(AdhocPackage* pkg);

    private:

};