#ifndef _COMMU_DEVICE_BASE_H_
#define _COMMU_DEVICE_BASE_H_

using std::string;

class CommuDeviceBase{
    public:
        virtual void OutputMessage(string message);
        virtual void SpinOnce();
        virtual bool HasNewChatting();  //Only New message from BleClient.
        virtual char* ReadChatting();
        virtual void WriteNotification(const char* notification);

    protected:        


};


#endif