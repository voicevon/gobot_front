// #include "app_cfg.h"
// #include "LuaTask.h"
// #include "lua.h"
// #include "lua/lualib.h"

// #include "lauxlib.h"
// #include "ltable.h"

// #include "utils.h"
// #include "bsp.h"
// #include "LuaTask.h"
// #include "LuaPlatform.h"

// #include "IOTask.h"
// #include "RFCommTask.h"
// #include "UartTask.h"
// #include "FlashAccess.h"
// #include "UartTask.h"

// void LoadPlatform(lua_State* l);

// static int LuaWaitEvent(lua_State* l);
// static int LuaResumeTask(lua_State* l);
// static int LuaRFProcess(lua_State* l);
// static int LuaUartDataProc(lua_State* l);

// static int LuaPrintf(lua_State* l);
// static int LuaDelay(lua_State* l);
// static int LuaGetTicks(lua_State* l);

// static int LuaRUNLEDFlash(lua_State* l);
// static int LuaDILEDFlash(lua_State* l);
// static int LuaDILEDON(lua_State* l);
// static int LuaDILEDOFF(lua_State* l);
// static int LuaDOLEDFlash(lua_State* l);
// static int LuaRFLEDFlash(lua_State* l);
// static int LuaUartProcess(lua_State* l);
// static int LuaNormalProc(lua_State* l);

// static int LuaUartSend(lua_State* l);
// static int LuaUartRecv(lua_State* l);

// static int LuaSendToHost(lua_State* l);
// static int LuaSendToHostWithFlag(lua_State* l);

// static int LuaGetInputData(lua_State* l);
// static int LuaGetInputByteArray(lua_State* l);
// static int LuaGetInputByUserData(lua_State* l);

// static int LuaRFRecv(lua_State* l);
// static int LuaRFResponse(lua_State* l);

// static int LuaSetLamp(lua_State *l);
// static int LuaGetRFData(lua_State *l);
// static int LuaResetSystem(lua_State *l);
// static int LuaComputeCRC(lua_State *l);

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

// static int LuaBitAnd(lua_State* l);
// static int LuaNumber2Bytes(lua_State* l);

// static const struct luaL_Reg BitFunc[] = 
// {
// 	{"And",			LuaBitAnd},
// 	{"Number2Bytes",LuaNumber2Bytes},
// 	{0,				0}
// };

// extern OS_FLAG_GRP * pFlagGrp;
// extern INT8U sendBuf[1024];
// extern INT8U recvBuf[1024];		//RF接收缓存

// extern DataCollectResult dataCollectResult;

// extern EnergyStruct energyStruct;

// extern UartRecvStruct uartRecvStruct;
// extern OS_EVENT* pUart4SendCompleted;

// lua_State* L = 0;
// extern LUAToHostStruct luaToHostStruct;

// void LuaInit(void)
// {
// 	luaToHostStruct.bChanged = FALSE;
// 	luaToHostStruct.len = 0;
// 	luaToHostStruct.pMutex = OSSemCreate(1);
	
//     L = luaL_newstate();
//     luaL_openlibs(L); 
//     luaL_newlib(L, RichonFunc);
//     lua_setglobal(L, "richon");   
    
// //	luaopen_bit32(L);	//LUA位运算支持库
// 	luaL_newlib(L, BitFunc);
// 	lua_setglobal(L, "Convert");
	
//     LoadPlatform(L);
// }

// void LoadPlatform(lua_State* l)
// {
// 	int i;
// 	int result;
// 	const char* str;
// 	char* pScript;
// 	const char* pDummyScript;
	
// 	BOOLEAN bHasScript = TRUE;
	
// 	const char* pGlobalVarDef = "IOTASK=3; RFTASK = 4; UARTTASK = 5; t=nil; count = 0; rfcount = 0; energy = nil";

// 	const char* taskManStr = "local coroutine = coroutine \
// 		function coroutine.xpresume(co) \
// 			local succeed, result = coroutine.resume(co) \
// 			if not succeed then \
// 				richon.Printf('LUA Error: ' .. tostring(result) .. '\\n\\r') \
// 			end\
// 			return succeed \
// 		end "
	
// 		"local table = table \
// 		TaskManager = {} \
// 		local waitingTasks = {} \
// 		local signals = {} "
// 		"function TaskManager.update(dt) \
// 			local waking = {} \
// 			for co, t in pairs(waitingTasks) do \
// 				if t.time and t.time > 0 then \
// 					t.time = t.time - dt \
// 					if t.time <= 0 then \
// 						table.insert(waking, co) \
// 					end\
// 				elseif t.func then \
// 					if t.func() then\
// 						table.insert(waking, co)\
// 					end\
// 				end\
// 			end\
// 			for _, co in ipairs(waking) do\
// 				waitingTasks[co] = nil \
// 				coroutine.xpresume(co) \
// 			end \
// 		end "
		
// 		"function TaskManager.wait(cond) \
// 			local co, result = coroutine.running() \
// 			if result == 1 then \
// 				richon.Printf('wait functions can only be called in task' .. '\\r\\n') \
// 				assert(co, 'wait functions can only be called in task\\r\\n') \
// 			end \
// 			if type(cond) == 'number' then \
// 				waitingTasks[co] = {time = cond * 400 / 1000} \
// 			elseif type(cond) == 'function' then \
// 				waitingTasks[co] = { func = cond } \
// 			elseif type(cond) == 'string' then \
// 				if signals[cond] == nil then \
// 					signals[cond] = { co } \
// 				else \
// 					table.insert(signals[cond], co) \
// 				end\
// 			end\
// 			return coroutine.yield() \
// 		end	"
		
// 		"function TaskManager.signal(sig) \
// 			local waitingSignalTasks = signals[sig] \
// 			if waitingSignalTasks then \
// 				signals[sig] = nil \
// 				for _, co in ipairs(waitingSignalTasks) do \
// 					coroutine.xpresume(co) \
// 				end \
// 			end \
// 		end "
		
// 		"function TaskManager.run(func) \
// 			local co = coroutine.create(func) \
// 			return coroutine.xpresume(co) \
// 		end "
		
// 		"function TaskManager.kill(co) \
// 			waitingTasks[co] = nil \
// 		end ";
// //		"define('TaskManager', TaskManager) ";
	
// 	const char* runStr ="waitTicks = 0 "
// 		"function run()"
// 			"local evt = richon.WaitEvent() \
// 			if evt == 0 then \
// 				local dt = waitTicks \
// 				waitTicks = richon.GetTicks() \
// 				dt = waitTicks - dt \
// 				TaskManager.update(dt) \
// 			end\
// 			if (evt & 0x01) == 1 then \
// 				TaskManager.signal('IOCHANGED') \
// 				richon.ResumeTask(IOTASK) \
// 			end\
// 			if (evt & 0x02) == 2 then \
// 				TaskManager.signal('RFRECV') \
// 			end\
// 			if (evt & 0x04) == 4 then \
// 				TaskManager.signal('UARTRECV') \
// 				richon.ResumeTask(UARTTASK) \
// 			end "
// //			"local t = Convert.Number2Bytes(74.0) \
// //			richon.Printf('volt bytes len is: ' .. tostring(#t) .. '\\n\\r') \
// //			for i = 1, #t do \
// //				richon.Printf('t' .. tostring(i) .. 'is ' .. tostring(t[i]) .. ' ') \
// //			end "
// 		"end ";

// 	luaL_dostring(l, pGlobalVarDef);		//全局变量定义区
// 	result = luaL_dostring(l, taskManStr);
// 	if(LUA_OK != result){
// 		printf("taskManStr loading error %d\n\r", result);
// 	}
	
// 	result = luaL_dostring(l, runStr);
// 	if(LUA_OK != result){
// 		printf("runStr loading error %d\n\r", result);
// 	}
	
// 	//首先要判断是否在特定的位置有脚本，默认情况下烧录仅一个循环程序
// 	//如果查看前八个字节都是FF则写入dummy脚本
// 	pDummyScript = "function DummyScript() \
// 		while true do \
// 			richon.Printf('Dummy\\n\\r') \
// 			TaskManager.wait(1000) \
// 			richon.DILEDFlash() \
// 			richon.UARTLEDFlash() \
// 		end \
// 	end "
// 	"TaskManager.run(DummyScript)";
	
// 	pScript = (char*)SCRIPT_BASEADDR;
// //	for(i = 0; i < 8; ++i){
// //		if((INT8U)pScript[i] == 0xFF){
// //			bHasScript = FALSE;
// //			break;
// //		}
// //	}
// 	bHasScript = FALSE;
	
// 	//当写入256个字节是出现问题，则后面没有0，则会死机，只能全部刷新Flash，为了解决这个问题，需要判断多次
// 	for(i = 1; i < 64; ++i){		//最多不能超过64个256，也就是16K
// 		if((INT8U)pScript[i * 256 - 1] == 0x00){
// 			bHasScript = TRUE;
// 			break;
// 		}
// 	}
	
// 	if((INT8U)pScript[strlen(pDummyScript) + 1] == 0x00){
// 		bHasScript = TRUE;
// 	}
		
// 	if(bHasScript == FALSE){
// 		//写入dummy脚本
// 		WriteScript((INT8U*)pDummyScript, strlen(pDummyScript) + 1, 0);
// 	}
	
// 	result = luaL_dostring(l, pScript);
// 	str = lua_tostring(L, -1);

// 	if(LUA_OK != result){
// 		printf("LuaPlatform dostring error %s\n\r", str);
// 	}
// }

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

// static int LuaComputeCRC(lua_State *l){
// 	int i, tmp;
// 	INT8U buf[64];
// 	INT16U  crc16;
	
	
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
	
// 	//计算CRC16
// 	crc16 = Cal_CRC(buf, tmp);
	
// 	//返回两个字节的数据
// 	lua_newtable(l);
		
// 	lua_pushinteger(l, crc16 & 0xFF);
// 	lua_rawseti(l, -2, 1);
// 	lua_pushinteger(l, (crc16 >> 8) & 0xFF);
// 	lua_rawseti(l, -2, 2);
	
// 	return 1;
// }



// /*
// 参数格式：LuaSetLamp(target, state);
// 	其中target有0,1,2
// 		state为0/1
// LUA调用方式：SetLamp(target, state)
// */
// static int LuaSetLamp(lua_State *l){
// 	int i;
// 	int target, state;
	
// 	i = lua_gettop(l);
// 	if(i == 2){
// 		target = lua_tointeger(l, 1);
// 		state = lua_tointeger(l, 2);
// 	}	
	
// 	switch(target){
// 		case 0:
// 			if(state)
// 				LAMP1_OFF();
// 			else
// 				LAMP1_ON();
// 			break;

// 		case 1:
// 			if(state)
// 				LAMP2_OFF();
// 			else
// 				LAMP2_ON();
// 			break;
			
// 		case 2:
// 			if(state)
// 				POWER_OFF();
// 			else
// 				POWER_ON();
// 			break;
// 		default:
// 			break;
// 	}
	
// 	return 0;
// }

// int LuaGetInputData(lua_State* l){	//把IOTask中的dataCollectResult以字节的形式传递到LUA中
// 	size_t iBytes;
// 	const char* p;
		
// 	iBytes = sizeof(DataCollectResult) - offsetof(DataCollectResult, cpuId);
	
// 	p = (const char*)(&((INT8U*)(&dataCollectResult))[offsetof(DataCollectResult, cpuId)]);
// 	lua_pushlstring(l, p, iBytes);
	
// 	return 1;
// }

// static int LuaGetInputByteArray(lua_State* l){
// 	size_t iBytes;
// 	const char* psrc;
// 	int i;

// 	iBytes = sizeof(DataCollectResult) - offsetof(DataCollectResult, cpuId);
// 	psrc = (const char*)(&((INT8U*)(&dataCollectResult))[offsetof(DataCollectResult, cpuId)]);
	
// 	lua_newtable(l);
// 	for(i = 0; i < iBytes; ++i){
// 		lua_pushinteger(l, *psrc);
// 		lua_rawseti(l, -2, i + 1);
// 		++psrc;
// 	}
	
// 	return 1;
// }

// static int LuaGetInputByUserData(lua_State* l){
// 	size_t iBytes;
// 	INT8U* p;
// 	const char* psrc;
	
// 	//iBytes = sizeof(DataCollectResult) + sizeof(INT16U);
// 	iBytes = sizeof(DataCollectResult) - offsetof(DataCollectResult, cpuId);
// 	psrc = (const char*)(&((INT8U*)(&dataCollectResult))[offsetof(DataCollectResult, cpuId)]);
// 	p = (INT8U*)lua_newuserdata(l, iBytes + 2);
// 	memcpy(p, (INT8U*)&iBytes, sizeof(INT16U));
// 	p += sizeof(INT16U);
// 	//memcpy(p, &dataCollectResult, sizeof(DataCollectResult));
// 	memcpy(p, psrc, iBytes);
	
// 	return 1;
// }

// static int LuaUartDataProc(lua_State* l){
// 	size_t iBytes;
// 	EnergyStruct* p;
	
// 	iBytes = sizeof(EnergyStruct);
// 	p = (EnergyStruct*)lua_newuserdata(l, iBytes);
// 	memcpy(p, &energyStruct, iBytes);
	
// 	energyStruct.bChanged = FALSE;
// 	return 1;
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

// typedef struct tagCacheData{
// 	INT8U len;
// 	INT8U buf[64];
// } CacheData;

// static CacheData cacheData;
// void CacheRFRecvData(INT8U *p, INT8U len){
// 	memset(&cacheData, 0, sizeof(cacheData));
// 	cacheData.len = len;
// 	memcpy(cacheData.buf, p, len);
// }

// static int LuaGetRFData(lua_State *l){
// 	INT8U i;
	
// 	if(cacheData.len < 64){
// 		lua_newtable(l);
// 		for(i = 0;i < cacheData.len; ++i){
// 			lua_pushinteger(l, cacheData.buf[i]);
// 			lua_rawseti(l, -2, i + 1);
// 		}
		
// 		return 1;
// 	}
	
// 	return 0;	
// }

// static int LuaRFRecv(lua_State* l){
// 	//获得从RF发送到LUA引擎中的命令
// 	INT16U i;
// 	CmdStruct* pCmd;
	
// 	pCmd = (CmdStruct*)recvBuf;
	
// 	//把主机发送过来的数据发送到LUA引擎
// 	lua_newtable(l);
// 	for(i = 0; i < pCmd->paramLen; ++i){
// 		lua_pushinteger(l, pCmd->param[i]);
// 		lua_rawseti(l, -2, i + 1);
// 	}
// 	return 1;
// }

// static int LuaRFResponse(lua_State* l){
// 	int i;
// 	INT16U len = 0;
// 	ResultStruct* pResult;
// 	pResult = (ResultStruct*)sendBuf;
	
// 	pResult->cmd = CMD_SENDTOLUA;
// 	pResult->status = 0;
	
// 	//判断数组长度
// 	i = lua_gettop(l);
// 	if(lua_istable(l, i)){
// 		lua_pushnil(l);
// 		while(lua_next(l, i) != 0){
// 			pResult->param[len] = lua_tointeger(l, -1);
// 			lua_remove(l, -1);
// 			++len;
// 		}
// 	}
	
// 	pResult->paramLen = len;
	
// 	return 0;
// }

// static int LuaSendToHost(lua_State* l){
// 	//向主机发送数据，传递过来的是字节数组
// 	INT8U err;
// 	int i;
// 	size_t tmp;
// 	INT8U buf[128];
// 	const char* p;
// 	UserDataStruct* pUserData;
	
// 	tmp = 0;
// 	memset(buf, 0, sizeof(buf));
// 	i = lua_gettop(l);
// 	if(lua_istable(l, i)){
// 		lua_pushnil(l);
// 		while(lua_next(l, i) != 0){
// 			buf[tmp] = lua_tointeger(l, -1);
// 			lua_remove(l, -1);
// 			++tmp;
// 		}
// 	}
	
// 	if(lua_isstring(l, i)){
// 		p = lua_tolstring(l, -1, &tmp);
// 		memcpy(buf, p, tmp);
// 	}
	
// 	if(lua_isuserdata(l, i)){
// 		pUserData = (UserDataStruct*)lua_touserdata(l, i);
// 		if(pUserData->len < sizeof(buf)){
// 			memcpy(buf, pUserData->buf, pUserData->len);
// 			tmp = pUserData->len;
// 		}
// 		else{
// 			printf("UserData is more then buf space\r\n");
// 		}
// 	}
	
// 	//在这里把数据放到RF任务可以访问的位置，并且互斥访问
// 	OSSemPend(luaToHostStruct.pMutex, 0, &err);
// 	if(err == OS_ERR_NONE){
// 		luaToHostStruct.bChanged = TRUE;
// 		luaToHostStruct.len = tmp;
// 		memcpy(luaToHostStruct.buf, buf, tmp);
// 		OSSemPost(luaToHostStruct.pMutex);
// 	}
// 	return 0;
// }

// static int LuaSendToHostWithFlag(lua_State* l){
// 	//向主机发送数据，传递过来的是字节数组
// 	INT8U err;
// 	int i;
// 	size_t tmp;
// 	INT8U buf[128];
// 	const char* p;
// 	UserDataStruct* pUserData;
// 	INT8U flag;
	
// 	memset(buf, 0, sizeof(buf));
// 	i = lua_gettop(l);
	
// 	flag = 0;
// 	if(i == 2){
// 		//得到第一个参数（flag）
// 		if(lua_isinteger(l, 1)){
// 			flag = lua_tointeger(l, 1);
// //			lua_remove(l, -1);
// 		}
// 	}
	
// 	buf[0] = flag;			//第一个字节是flag
// 	tmp = 1;
	
// 	if(lua_istable(l, i)){
// 		lua_pushnil(l);
// 		while(lua_next(l, i) != 0){
// 			buf[tmp] = lua_tointeger(l, -1);
// 			lua_remove(l, -1);
// 			++tmp;
// 		}
// 	}
	
// 	if(lua_isstring(l, i)){
// 		p = lua_tolstring(l, -1, &tmp);
// 		memcpy(&buf[1], p, tmp);
// 		tmp = tmp + 1;
// 	}
	
// 	if(lua_isuserdata(l, i)){
// 		pUserData = (UserDataStruct*)lua_touserdata(l, i);
// 		if(pUserData->len < sizeof(buf)){
// 			memcpy(&buf[1], pUserData->buf, pUserData->len);
// 			tmp = pUserData->len + 1;
// 		}
// 		else{
// 			printf("UserData is more then buf space\r\n");
// 		}
// 	}
	
// 	//在这里把数据放到RF任务可以访问的位置，并且互斥访问
// 	OSSemPend(luaToHostStruct.pMutex, 0, &err);
// 	if(err == OS_ERR_NONE){
// 		luaToHostStruct.bChanged = TRUE;
// 		luaToHostStruct.len = tmp;
// 		memcpy(luaToHostStruct.buf, buf, tmp);
// 		OSSemPost(luaToHostStruct.pMutex);
// 	}
// 	return 0;
// }


// int LuaRFProcess(lua_State* l){
// 	int i;
// 	ResultStruct* pResult;
	
// 	DataCollectResult* p;
// 	pResult = (ResultStruct*)sendBuf;
// 	p = (DataCollectResult*)lua_touserdata(l, 1);
// 	if(NULL == p){					//传递过来的userdata为nil
// 		pResult->cmd = CMD_QUERYDATA;	//0x00;	///pCmd->cmd;
//         pResult->status = 0;
//         pResult->paramLen = 0;		
// 		return 0;
// 	}
// 	//在这里组织数据
// 	if(p->bChanged){
// 		pResult->cmd = CMD_QUERYRESULT;
// 		pResult->status = 0;
// 		pResult->paramLen = sizeof(DataCollectResult) - offsetof(DataCollectResult, cpuId);
// 		for(i = 0; i < pResult->paramLen; ++i)
// 		{
// 			pResult->param[i] = (&(((INT8U*)p)[offsetof(DataCollectResult, cpuId)]))[i];
// 		}
//         p->bChanged = FALSE;
// 		printf("send io data\r\n");
// 	}
// 	else{
// 		pResult->cmd = CMD_QUERYDATA;	//0x00;	///pCmd->cmd;
//         pResult->status = 0;
//         pResult->paramLen = 0;
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

// int LuaUartProcess(lua_State* l){
// 	int i;
// 	ResultStruct* pResult;
// 	EnergyStruct* p;
	
// 	pResult = (ResultStruct*)sendBuf;
// 	p = (EnergyStruct*)lua_touserdata(l, 1);
// 	if(NULL == p){
// 		pResult->cmd = CMD_QUERYDATA;
//         pResult->status = 0;
//         pResult->paramLen = 0;		
// 		return 0;
// 	}
	
// 	if(p->bChanged){
// 		pResult->cmd = CMD_QUERYRESULT;
// 		pResult->status = 0;
// 		pResult->paramLen = sizeof(EnergyStruct) - offsetof(EnergyStruct, volt);
// 		for(i = 0; i < pResult->paramLen; ++i){
// 			pResult->param[i] = (&(((INT8U*)p)[offsetof(EnergyStruct, volt)]))[i];
// 		}
// 		p->bChanged = FALSE;
// 	}
// 	else{
// 		pResult->cmd = CMD_QUERYDATA;	//0x00;	///pCmd->cmd;
//         pResult->status = 0;
//         pResult->paramLen = 0;
// 	}
	
// 	return 0;
// }


// void TestInnerFun(void){	
// 	lua_getglobal(L, "innerfun");
// 	lua_pushnumber(L, 2);
// 	lua_pushnumber(L, 3);
	
// 	if(lua_pcall(L, 2, 1, 0) != 0){	//lua_pcall参数(lua_Sate, int nargs, int nresults, int errfunc)，最后一个参数是出错时自动按照
// 									//如下格式调用：(L, errfun)(errmsg)。如果不用最后一个参数可以使用lua_call(lua_State, int nargs, int nresults)；
// 		return;
// 	}
	
// 	if(!lua_isnumber(L, -1)){
// 		return;
// 	}
	
// 	lua_tonumber(L, -1);
	
// 	lua_pop(L, 1);		//为了平衡栈，需要把返回值弹栈
// }

// int LuaPrintf(lua_State* l){
// 	int i;
// 	const char* p;
// 	char buf[64];
	
// 	size_t len;
	
// 	memset(buf, 0, sizeof(buf));
// 	i = lua_gettop(l);
// 	if(i == 1){
// 		p = lua_tolstring(l, 1, &len);
// 		if(len > 0)
// 			printf("%s", p);	//解决编译警告：warning format string is not string literal
// 	}
	
// 	return 0;
// }
// int LuaDelay(lua_State* l)
// {
//     int i;
//     int ms;
//     i = lua_gettop(l);
//     if(i == 1)
//     {
//         if(lua_isinteger(l, 1))
//         {
//             ms = lua_tointeger(l, 1);
//             OSTimeDlyHMSM(0, 0, 0, ms);
//         }            
//     }

//     return 0;
// }

// int LuaGetTicks(lua_State* l){
// 	int ticks;
// 	ticks = OSTimeGet();
// 	lua_pushinteger(l, ticks);
// 	return 1;
// }

// int LuaRUNLEDFlash(lua_State* l)
// {
//     static BOOLEAN isLight = TRUE;
//     if(isLight)
//     {
//         RUNLED_ON();
//     }
//     else
//     {
//         RUNLED_OFF();
//     }
//     isLight = !isLight;
//     return 0;
// }

// int LuaDILEDFlash(lua_State* l)
// {
//     static BOOLEAN isLight = TRUE;
//     if(isLight)
//     {
//         DILED_ON();
//     }
//     else
//     {
//         DILED_OFF();
//     }
//     isLight = !isLight;
//     return 0;
// }

// int LuaDILEDON(lua_State* l){
// 	DILED_ON();
// 	return 0;
// }

// int LuaDILEDOFF(lua_State* l){
// 	DILED_OFF();
// 	return 0;
// }

// int LuaDOLEDFlash(lua_State* l)
// {
//     static BOOLEAN isLight = TRUE;
//     if(isLight)
//     {
//         DOLED_ON();
//     }
//     else
//     {
//         DOLED_OFF();
//     }
//     isLight = !isLight;
//     return 0;
// }

// int LuaRFLEDFlash(lua_State* l)
// {
//     static BOOLEAN isLight = TRUE;
//     if(isLight)
//     {
//         RFLED_ON();
//     }
//     else
//     {
//         RFLED_OFF();
//     }
//     isLight = !isLight;
//     return 0;
// }

// static int LuaBitAnd(lua_State* l){
// 	//两个参数进行位与后返回
// 	int i;
// 	int result;
	
// 	i = lua_gettop(l);
// 	if(i == 2){
// 		result = lua_tointeger(l, 1);
// 		result = result & lua_tointeger(l, 2);
// 	}
	
// 	lua_pushinteger(l, result);
	
// 	return 1;
// }

// /*
// 在LUA中按照IEEE-754标准实现float到byte数组的转换留待日后实现
// */
// static int LuaNumber2Bytes(lua_State* l){
// 	int i;
// 	INT8U buf[16];
// 	float param;
		
// 	memset(buf, 0, sizeof(buf));
// 	i = lua_gettop(l);
	
// 	if(i == 1){
// 		//if(lua_isnumber(l, 1)){
// 		if(lua_type(l, -1) == LUA_TNUMBER){
// 			param = lua_tonumber(l, -1);
			
// 			memcpy(buf, (INT8U*)&param, sizeof(float));
			
// 			lua_newtable(l);
// 			for(i = 0; i < sizeof(float); ++i){
// 				lua_pushinteger(l, buf[i]);
// 				lua_rawseti(l, -2, i + 1);
// 			}
			
// 			return 1;
// 		}
// 	}
	
// 	return 0;
// }

// static int LuaResetSystem(lua_State *l){
// 	NVIC_SystemReset();
// 	return 0;
// }



