#include "text_message_queue.h"

TextMessageQueue::TextMessageQueue(){
    this->_Init("TextMessageQueue", COMMAND_QUEUE_SIZE, this->__all_commands);
    // this->_Init("TextMessageQueue", COMMAND_QUEUE_SIZE, &this->__all_commands[0]);
}

int TextMessageQueue::AppendCommand(String command){
    TextMessageLine* command_text =  this->GetRoom_ForDeposit();
    command_text->CopyFrom(command.c_str());
    this->Deposit();
    return 1;
};

int TextMessageQueue::AppendCommand(const char* command){
    // Logger::Debug("TextMessageQueue::AppendGcodeCommand(const char* command)");
    // this->PrintOut("caller is:  TextMessageQueue::AppendGcodeCommand(const char* command)");

    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("TextMessageQueue::AppendGcodeCommand() with auto-length,  queue is full");
        this->PrintOut("TextMessageQueue::AppendGcodeCommand(const char* command)");
        return -1;
    }


    // Logger::Print("TextMessageQueue::AppendGcodeCommand(const char* command) point 51", 51);
    TextMessageLine* command_text = this->GetRoom_ForDeposit();
    // Logger::Print("TextMessageQueue::AppendGcodeCommand(const char* command) point  52", 52);
    // Logger::Print("command", command->command);
    int res = command_text->CopyFrom(command);
    if (res == 1){
        this->Deposit();
        // command_text->PrintFlat("caller::TextMessageQueue::AppendGcodeCommand()");
        // command_text = this->WithdrawTailElement();
        // command_text->PrintFlat("aaaaaaaaaaaaaa  WithdrawTailElement()");
        return res;
    }


    // Logger::Error("TextMessageQueue::AppendGcodeCommand() with auto-length,  over-size   point -2");
    // Logger::Print("REPRAP_GCODE_MAX_SIZE",  REPRAP_GCODE_MAX_SIZE);
    // Logger::Print("command", command);
    return -2;
}


int TextMessageQueue::AppendCommand(const char* payload, int length){
    // return this->AppendMessage(payload, length);
    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("TextMessageQueue::AppendGcodeCommand() queue is full");
        return -1;
    }
    if (length > REPRAP_GCODE_MAX_SIZE){
        Logger::Error("TextMessageQueue::AppendGcodeCommand() payload is over-size");
        return -2;
    }
    TextMessageLine* command_text = this->GetRoom_ForDeposit();
    int result = command_text->CopyFrom(payload, length);
    if (result == C_STRING_OK){
        // command_text->PrintFlat("appended, please verify");
        this->Deposit();
        return 1;
    }
};

int TextMessageQueue::AppendCommand(TextMessageLine* command_text){
    // return this->AppendMessage(payload, length);
    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("TextMessageQueue::AppendGcodeCommand() queue is full");
        return -1;
    }
    // if (length > REPRAP_GCODE_MAX_SIZE){
    //     Logger::Error("TextMessageQueue::AppendGcodeCommand() payload is over-size");
    //     return -2;
    // }
    TextMessageLine* command_text_in_queue = this->GetRoom_ForDeposit();
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


void TextMessageQueue::PrintOut_TextMessageLine(const char* title){
    Logger::Info(title);
    // for(int i=0; i<GCODE_QUEUE_SIZE;i++){
    //     Serial.print(i);
    //     Serial.print("\t");
    //     this->__all_commands[i].PrintFlat("caller::TextMessageQueue::PrintOut_GcddeText()");
    //     Serial.print("\t");
    //     this->__all_elements[i].PrintFlat("caller::TextMessageQueue::PrintOut_GcddeText()");
    //     Serial.println("");
    // }
}
