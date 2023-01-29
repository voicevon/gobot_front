#pragma once

#include "adhoc_base.h"

class AdhocSniffer: public AdhocBase{
    public:
        AdhocPackage rx_package;  // TODO:  put to a rx_queue.
        bool is_new_rx_package;
        void onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) override; 
        void SpinOnce() override;
        void Init(uint8_t my_app_node_id) override;
        
    private:

};