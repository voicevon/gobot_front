#ifndef _COMMU_DEVICE_BASE_H_
#define _COMMU_DEVICE_BASE_H_

#include <string>


class CommuDeviceBase{
    public:
        virtual void OutputMessage(std::string message);
        virtual void WriteNotification(std::string notification);
        // virtual void OutputMessage(char* message){};
        virtual void Init();
        virtual void SpinOnce();
        virtual bool HasNewChatting();  //Only New message from BleClient.
        virtual char* ReadChatting();

    protected:        

};


#endif