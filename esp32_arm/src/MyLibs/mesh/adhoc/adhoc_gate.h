#pragma once
#include "adhoc_base.h"

class AdhocGate: public AdhocBase{
    public:
        AdhocPackage rx_package;  // TODO:  put to a rx_queue.
        bool onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) override;
        bool received_new_app_package;

        void Init(uint8_t my_node_id) override;

    protected:

    private:

};