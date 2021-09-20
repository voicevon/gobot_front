#ifndef _COMMU_DEVICE_BASE_H_
#define _COMMU_DEVICE_BASE_H_

#include <string>


class CommuDeviceBase{
    public:
        virtual void OutputMessage(std::string message);
        virtual void SpinOnce();
        virtual bool HasNewChatting();  //Only New message from BleClient.
        virtual char* ReadChatting();
        virtual void WriteNotification(const char* notification);

    protected:        


};


#endif