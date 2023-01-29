#pragma once
#include "adhoc_base.h"

class AdhocGate: public AdhocBase{
    public:
        AdhocPackage rx_package;  // TODO:  put to a rx_queue.
        void onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) override;
        bool is_new_rx_package;

        void SpinOnce() override;
        void Init() override;

    protected:
        // uint8_t _GetMyHop() override {return 1;};

    private:

};