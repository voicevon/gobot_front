#pragma once
#include "bolt_nut.h"
#include "WiFi.h"
#include <esp_now.h>

#define ROUTER_TABLE_ROWS  16


class AdhocBase{
    public:
        virtual void onReceived(const uint8_t * mac, const uint8_t *incomingData, int len);
        virtual void SpinOnce();
        virtual void Init();
        void Send(AdhocPackage* pkg);


    protected:
        uint8_t __my_hop;
        uint8_t __my_mac_addr[6];
        uint32_t __time_count_up;
        AdhocPackage __orphan_package;

    private:

};