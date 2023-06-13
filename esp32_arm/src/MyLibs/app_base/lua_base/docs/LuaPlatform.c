

// static const struct luaL_Reg RichonFunc[] =
// {
// 	{"WaitEvent",	LuaWaitEvent},
// 	{"ResumeTask",  LuaResumeTask},
// 	{"GetInputData",LuaGetInputData},
// 	{"UartDataProc",LuaUartDataProc},
// 	{"RFProcess",	LuaRFProcess},
// 	{"UARTProcess", LuaUartProcess},
// 	{"NormalProcess", LuaNormalProc},
// 	{"Printf",		LuaPrintf},
// 	{"GetTicks",	LuaGetTicks},
//     {"Delay"   , 	LuaDelay},
//     {"RUNLEDFlash", LuaRUNLEDFlash},
//     {"DILEDFlash",  LuaDILEDFlash},
// 	{"DOLEDFlash",  LuaDOLEDFlash},
// 	{"DILEDON",		LuaDILEDON},
// 	{"DILEDOFF",	LuaDILEDOFF},
//     {"UARTLEDFlash",  LuaDOLEDFlash},
//     {"RFLEDFlash",  LuaRFLEDFlash},
// 	{"UartSend",	LuaUartSend},
// 	{"UartRecv", 	LuaUartRecv},
// 	{"SendToHost",  LuaSendToHost}, 
// 	{"SendToHostWithFlag",  LuaSendToHostWithFlag}, 
// 	{"GetInputByteArray", LuaGetInputByteArray},
// 	{"GetInputByUserData", LuaGetInputByUserData},
// 	{"RFRecv",		LuaRFRecv},
// 	{"RFResponse",  LuaRFResponse},
// 	{"SetLamp",		LuaSetLamp},
// 	{"GetRFData",	LuaGetRFData},
// 	{"ResetSystem", LuaResetSystem},
// 	{"ComputeCRC",	LuaComputeCRC},
//     {0,           	0}
// };


// int LuaWaitEvent(lua_State* l){
// 	INT8U err;
// 	OS_FLAGS flags;
	
// 	flags = OSFlagPend(pFlagGrp, (OS_FLAGS)ALL_FLAGS, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 4, &err);	//50毫秒一次超时
	
// 	//把flags发送到LUA中
// 	lua_pushinteger(l, flags);
// 	return 1;
// }

// int LuaResumeTask(lua_State* l){
// 	int i;
// 	int pri;
//     i = lua_gettop(l);
//     if(i == 1)
//     {
//         if(lua_isinteger(l, 1))
//         {
//             pri = lua_tointeger(l, 1);
			
// 			OSTaskResume(pri);
//         }            
//     }
	
//     return 0;	
// }







// static int LuaUartSend(lua_State* l){
// 	int i, tmp;
// 	INT8U buf[64];
// 	INT16U semcount;
	
// 	//获得LUA传递过来的数组
// 	tmp = 0;
// 	i = lua_gettop(l);
// 	if(lua_istable(l, i)){
// 		lua_pushnil(l);
		
// 		while(lua_next(l, i) != 0){
// 			buf[tmp] = lua_tointeger(l, -1);
// 			lua_remove(l, -1);
// 			++tmp;
// 		}
// 	}
	
// 	do{
// 		semcount = OSSemAccept(pUart4SendCompleted);
// 	}while(semcount > 0);
		
// 	SendToUart4(buf, tmp);
	
// 	return 0;
// }

// static int LuaUartRecv(lua_State* l){
// 	INT8U i;
	
// 	if(uartRecvStruct.len > 0){
// 		//拷贝UART接收缓冲区数据到LUA虚拟机
// 		lua_newtable(l);
// 		for(i = 0; i < uartRecvStruct.len; ++i){
// 			lua_pushinteger(l, uartRecvStruct.buf[i]);
// 			lua_rawseti(l, -2, i + 1);
// 		}
// 		return 1;
// 	}
	
// 	return 0;
// }









// static int LuaNormalProc(lua_State* l){
// 	ResultStruct* pResult;
// 	pResult = (ResultStruct*)sendBuf;
// 	pResult->cmd = CMD_QUERYDATA;
// 	pResult->status = 0;
// 	pResult->paramLen = 0;
	
// 	return 0;
// }





