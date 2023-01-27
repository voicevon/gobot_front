# pragma once
#include "stdint.h"

#define ROUTER_TABLE_ROWS  16

class Neibour{
    public:
        uint8_t id;   // 0 = empty 
        uint8_t hop;
        char mac_addr[6];
};

class AdhocRouter{
    public:
        void Init();
        void SpinOnce();
        void BroadcastMyMacAddr();

    protected:


    private:
        void __Broadcast_Iam_Orphan();
        void __GetMyMacAddr(uint8_t* mac);

        void __onReceived_Orphan();
        Neibour __my_neibours[ROUTER_TABLE_ROWS];
        uint8_t __my_hop = 0xff;


};