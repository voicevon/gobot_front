# pragma once
#include "adhoc_base.h"


class AdhocRouter: public AdhocBase{
    public:
        void onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) override;
        void Init() override;
        void SpinOnce() override;
        bool IsJoined_Mesh();
    protected:

        // virtual uint8_t _GetMyHop();
        // void __send_out(AdhocPackage* package);
        // void __Broadcast_Iam_Orphan();

    private:
        Neibour __my_neibours[ROUTER_TABLE_ROWS];
        Neibour* __search_neibour(uint8_t * mac_addr);
        Neibour* __find_blank_neibour();
        int8_t __forward_to = -1;   // the index in routing_table.

        void __sniff_air_package(const uint8_t * mac, AdhocPackage* incoming_package);

};