# pragma once
#include "bolt_nut.h"

#define ROUTER_TABLE_ROWS  16



class AdhocRouter{
    public:
        void Init();
        void SpinOnce();
        void BroadcastMyMacAddr();

    protected:


    private:
        static void __onReceived(const uint8_t * mac, const uint8_t *incomingData, int len);
        Neibour __my_neibours[ROUTER_TABLE_ROWS];
        void __send_out(Package* package);

        void __Broadcast_Iam_Orphan();
        // void __GetMyMacAddr(uint8_t* mac);

        uint8_t __my_hop = 0xff;
        uint32_t __time_count_up;


};