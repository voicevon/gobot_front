# pragma once
#include "adhoc_base.h"


class AdhocRouter: public AdhocBase{
    public:
        bool onReceived(const uint8_t * sender_mac, const uint8_t *incomingData, int len) override;
        void Init(uint8_t my_app_node_id) override;
        bool HasJointNetwork() {return __my_leader != NULL;};
    protected:


    private:
        Neibour __my_neibours[ROUTER_TABLE_ROWS]; 
        Neibour* __search_neibour(uint8_t * mac_addr);
        Neibour* __find_blank_neibour();
        Neibour* __my_leader;

        void __sniff_air_package(const uint8_t * mac, AdhocPackage* incoming_package);
        void __append_to_neibours(uint8_t * mac, AdhocPackage* incoming_package);


};