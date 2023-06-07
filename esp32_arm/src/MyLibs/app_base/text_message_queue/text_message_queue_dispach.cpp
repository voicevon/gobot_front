// #include "text_message_queue_dispach.h"

// void MessageQueueDispacher::SpinOnce(){
//     TextMessageLine* message =  __text_message_queue->GetWithdrawTailElement(false);
//     if (message->IsPrefix("gcd:")){
//         GcodeQueue* gcode_queue = __robot->GetGcodeQueue();
//         if (gcode_queue->GetFreeBuffersCount() > 3){
//             // GcodeText* gcode = gcode_queue->GetRoom_ForDeposit();
//             // gcode = message.
//         }
//     }
// }