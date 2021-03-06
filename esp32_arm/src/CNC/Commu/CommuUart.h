#pragma once

#include "CommuDeviceBase.h"

class CommuUart: public CommuDeviceBase{
    public:
        void OutputMessage(std::string message) override;
        void WriteNotification(std::string notification) override;
        // void OutputMessage(char* message) override;
        void Init() override;
        void SpinOnce() override;
        bool HasNewChatting() override;  //Only New message from BleClient.
        char* ReadChatting() override;

    private:
        char RxBuffer[50];
        char* pRxBuffer;

};