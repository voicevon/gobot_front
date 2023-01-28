# pragma once
#include "bolt_nut.h"
#include "WiFi.h"
#include <esp_now.h>

#define ROUTER_TABLE_ROWS  16

class AdhocRouter{
    public:
        // static AdhocRouter& Instance(){
        //     static AdhocRouter instance;
        //     return instance;
        // }
        void onReceived(const uint8_t * mac, const uint8_t *incomingData, int len);
        
        void Init();
        void SpinOnce();
        void BroadcastMyMacAddr();

    protected:


    private:
        Neibour __my_neibours[ROUTER_TABLE_ROWS];
        void __send_out(Package* package);

        void __Broadcast_Iam_Orphan();
        // void __GetMyMacAddr(uint8_t* mac);

        uint8_t __my_hop;
        uint32_t __time_count_up;

        void broadcast(const uint8_t* message, uint8_t length);



};