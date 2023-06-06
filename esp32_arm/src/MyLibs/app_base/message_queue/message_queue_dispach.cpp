#include "message_queue_dispach.h"

void MessageQueueDispacher::SpinOnce(){
    CommandText* message =  __message_queue->GetWithdrawTailElement(false);
    if (message->IsPrefix("gcd:")){
        GcodeQueue* gcode_queue = __robot->GetGcodeQueue();
        if (gcode_queue->GetFreeBuffersCount() > 3){
            // GcodeText* gcode = gcode_queue->GetRoom_ForDeposit();
            // gcode = message.
        }
    }
}