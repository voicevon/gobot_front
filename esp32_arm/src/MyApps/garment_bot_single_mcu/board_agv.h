#pragma once

#include "MyLibs/board_base.h"

class BoardPart_Agv: public BoardbaseAgv{
    public:
        BoardPart_Agv();
        void Init(Adafruit_MCP23X17* mcp_23018){this->__mcp23018=mcp_23018;};

    private:
        Adafruit_MCP23X17* __mcp23018;


};
