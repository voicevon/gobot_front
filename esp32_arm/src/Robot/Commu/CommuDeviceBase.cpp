#include "CommuDeviceBase.h"


CommuDeviceBase::CommuDeviceBase(){
    for(int i=0; i<20; i++){
        RxBuffer[i] = 65;
    }
    RxBuffer[20] = 0;
    pRxBuffer = &RxBuffer[0];
}