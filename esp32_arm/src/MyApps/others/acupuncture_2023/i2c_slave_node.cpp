#include "i2c_slave_node.h"
#include "MyLibs/basic/logger.h"



void I2C_SlaveNode::Init(uint8_t cell_id,  uint8_t installed_channel_count){


}


// bool I2C_SlaveNode::HasUpdate(){
//     bool has_update = false;
//     // for(int i=0; i<2; i++){
//     //     if (LastFlags[i] != CurrentFlags[i]){
//     //         has_update = true;
//     //     }
//     // }
//     return has_update;
// }



// bool I2C_SlaveNode::CompareCurrentAndLast(){

//     return _has_update;
// }

// void I2C_SlaveNode::CopyCurrentToLast(){
//     for(int i =0; i<4; i++){
//         this->LastFlags[i] = this->CurrentFlags[i];
//     }
// }

// // paras: bit_index=[0..15]
// bool I2C_SlaveNode::IsBitUpdated(int bit_index){
//     int byte_index = 0;
//     if (bit_index>=8){
//         byte_index++;
//         bit_index -=8;
//     }
//     int bit_mask = (CurrentFlags[byte_index] ^ LastFlags[byte_index]) & (1<<bit_index);
//     if (bit_mask > 0){
//         // Logger::Debug("I2C_SlaveNode::IsBitUpdated()");
//         // Logger::Print("Cell address", Address);
//         // Logger::Print("Current Flag 0", CurrentFlags[0]);
//         // Logger::Print("Last Flag 0", LastFlags[0]);
//         // Logger::Print("Current Flag 1", CurrentFlags[1]);
//         // Logger::Print("Last Flag 1", LastFlags[1]);
//         // Logger::Print("bit_mask", bit_mask);
//         // Logger::Print("Updated Byte index", byte_index);
//         // Logger::Print("Updated Bit_index", bit_index);
//         return true;
//     }
//     return false;
// }
const char* I2C_SlaveNode::GetMqttPayload(int bit_index){
    int byte_index = 0;
    if (bit_index >= 8){
        byte_index++;
        bit_index -= 8;
    }
    // if (CurrentFlags[byte_index] & (1<<bit_index))
    //     return "ON";
    return "OFF";
}


