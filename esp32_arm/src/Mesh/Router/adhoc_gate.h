#pragma once
#include "adhoc_base.h"

class AdhocGate: public AdhocBase{
    public:
        AdhocPackage rx_package;  // TODO:  put to a rx_queue.
        bool onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) override;
        bool has_got_new_rx_package;

        void Init(uint8_t my_app_node_id) override;

    protected:

    private:

};