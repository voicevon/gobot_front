# pragma once
#include "adhoc_base.h"


class AdhocRouter: public AdhocBase{
    public:
        void onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) override;
        void Init(uint8_t my_app_node_id) override;
        void SpinOnce() override;
        // bool IsJoined_Mesh();

    protected:


    private:
        Neibour __my_neibours[ROUTER_TABLE_ROWS];   // index 0 is the leader
        Neibour* __search_neibour(uint8_t * mac_addr);
        Neibour* __find_blank_neibour();
        // Neibour* __get_temperory_leader();
        Neibour* __my_leader;
        // int8_t __hop_leader_index = -1;   // the index in routing_table.

        void __sniff_air_package(const uint8_t * mac, AdhocPackage* incoming_package);

};