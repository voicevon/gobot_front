#include "command_queue.h"

CommandQueue::CommandQueue(){
    this->_Init("CommandQueue", COMMAND_QUEUE_SIZE, this->__all_commands);
    // this->_Init("CommandQueue", COMMAND_QUEUE_SIZE, &this->__all_commands[0]);
}

int CommandQueue::AppendCommand(String command){
    CommandText* command_text =  this->GetRoom_ForDeposit();
    command_text->CopyFrom(command.c_str());
    this->Deposit();
    return 1;
};

int CommandQueue::AppendCommand(const char* command){
    // Logger::Debug("CommandQueue::AppendGcodeCommand(const char* command)");
    // this->PrintOut("caller is:  CommandQueue::AppendGcodeCommand(const char* command)");

    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("CommandQueue::AppendGcodeCommand() with auto-length,  queue is full");
        this->PrintOut("CommandQueue::AppendGcodeCommand(const char* command)");
        return -1;
    }


    // Logger::Print("CommandQueue::AppendGcodeCommand(const char* command) point 51", 51);
    CommandText* command_text = this->GetRoom_ForDeposit();
    // Logger::Print("CommandQueue::AppendGcodeCommand(const char* command) point  52", 52);
    // Logger::Print("command", command->command);
    int res = command_text->CopyFrom(command);
    if (res == 1){
        this->Deposit();
        // command_text->PrintFlat("caller::CommandQueue::AppendGcodeCommand()");
        // command_text = this->WithdrawTailElement();
        // command_text->PrintFlat("aaaaaaaaaaaaaa  WithdrawTailElement()");
        return res;
    }


    // Logger::Error("CommandQueue::AppendGcodeCommand() with auto-length,  over-size   point -2");
    // Logger::Print("REPRAP_GCODE_MAX_SIZE",  REPRAP_GCODE_MAX_SIZE);
    // Logger::Print("command", command);
    return -2;
}


int CommandQueue::AppendCommand(const char* payload, int length){
    // return this->AppendMessage(payload, length);
    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("CommandQueue::AppendGcodeCommand() queue is full");
        return -1;
    }
    if (length > REPRAP_GCODE_MAX_SIZE){
        Logger::Error("CommandQueue::AppendGcodeCommand() payload is over-size");
        return -2;
    }
    CommandText* command_text = this->GetRoom_ForDeposit();
    int result = command_text->CopyFrom(payload, length);
    if (result == C_STRING_OK){
        // command_text->PrintFlat("appended, please verify");
        this->Deposit();
        return 1;
    }
};

int CommandQueue::AppendCommand(CommandText* command_text){
    // return this->AppendMessage(payload, length);
    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("CommandQueue::AppendGcodeCommand() queue is full");
        return -1;
    }
    // if (length > REPRAP_GCODE_MAX_SIZE){
    //     Logger::Error("CommandQueue::AppendGcodeCommand() payload is over-size");
    //     return -2;
    // }
    CommandText* command_text_in_queue = this->GetRoom_ForDeposit();
    command_text_in_queue->CopyFrom(command_text->GetChars());
    this->Deposit();
    return 1;
    // int result = command_text->CopyFrom(payload, length);
    // if (result == GCODE_TEXT_OK){
    //     // command_text->PrintFlat("appended, please verify");
    //     this->Deposit();
    //     return 1;
    // }
};


void CommandQueue::PrintOut_CommandText(const char* title){
    Logger::Info(title);
    // for(int i=0; i<GCODE_QUEUE_SIZE;i++){
    //     Serial.print(i);
    //     Serial.print("\t");
    //     this->__all_commands[i].PrintFlat("caller::CommandQueue::PrintOut_GcddeText()");
    //     Serial.print("\t");
    //     this->__all_elements[i].PrintFlat("caller::CommandQueue::PrintOut_GcddeText()");
    //     Serial.println("");
    // }
}
