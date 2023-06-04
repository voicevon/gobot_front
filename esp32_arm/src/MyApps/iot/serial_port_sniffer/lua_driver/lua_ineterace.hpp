// int LuaWaitEvent(){

// }
// void LuaResumeTask()
__UINT64_TYPE__ LuaUartSend(uint8_t port_index, char* sending_buffer, __UINT16_TYPE__ sending_len){

    return 0;
}


	{"ResumeTask",  LuaResumeTask},
	{"GetInputData",LuaGetInputData},
	{"UartDataProc",LuaUartDataProc},
	{"RFProcess",	LuaRFProcess},
	{"UARTProcess", LuaUartProcess},
	{"NormalProcess", LuaNormalProc},
	{"Printf",		LuaPrintf},
	{"GetTicks",	LuaGetTicks},
    {"Delay"   , 	LuaDelay},
    {"RUNLEDFlash", LuaRUNLEDFlash},
    {"DILEDFlash",  LuaDILEDFlash},
	{"DOLEDFlash",  LuaDOLEDFlash},
	{"DILEDON",		LuaDILEDON},
	{"DILEDOFF",	LuaDILEDOFF},
    {"UARTLEDFlash",  LuaDOLEDFlash},
    {"RFLEDFlash",  LuaRFLEDFlash},
	{"UartSend",	LuaUartSend},
	{"UartRecv", 	LuaUartRecv},
	{"SendToHost",  LuaSendToHost}, 
	{"SendToHostWithFlag",  LuaSendToHostWithFlag}, 
	{"GetInputByteArray", LuaGetInputByteArray},
	{"GetInputByUserData", LuaGetInputByUserData},
	{"RFRecv",		LuaRFRecv},
	{"RFResponse",  LuaRFResponse},
    {0,           0}
};