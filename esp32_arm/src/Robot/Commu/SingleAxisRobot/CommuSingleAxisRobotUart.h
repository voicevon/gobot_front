#pragma once

#include "../CommuDeviceBase.h"

class CommuSingleAxisRobotUart: public CommuDeviceBase{
    public:
        void OutputMessage(std::string message) override;
        void SpinOnce()override;
        bool HasNewChatting()override;  //Only New message from BleClient.
        char* ReadChatting()override;
        void WriteNotification(const char* notification)override;
};