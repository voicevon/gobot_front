// extern "C"{

// lua_State* L = 0;

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "task_master_uart.h"
#include "task_slave_uart.h"
#include "api_mqtt.h"
#include "api_leds.h"

#include "api_rtos.h"
#include "global_const.h"

static const struct luaL_Reg RichonFunc[] =
{
	{"WaitEvent",	LuaWaitEvent},
	{"ResumeTask",  LuaResumeTask},
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
	{"SetLedPower",	    SetLedPower},
	{"UartSendMaster",	LuaUartSend_Master},
	{"UartReadMaster", 	LuaUartRecv_Master},
	{"UartSendSlave",	LuaUartSend_Slave},
	{"UartReadSlave", 	LuaUartRecv_Slave},

	{"MqttPublish", 	LuaMqttPublish},
	{"MqttSubscribe", 	LuaMqttSubscribe},
	// {"MqttReadTopic", 	LuaMqttReadFirstTopic},
	{"MqttReadPayload",	LuaMqttReadPayload},
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
    {0,           	0}
};






void LoadPlatform(lua_State* L){
// 	luaL_dostring(l, pGlobalVarDef);		//全局变量定义区
	// int result = luaL_dostring(lua_State* l, taskManStr);
// 	if(LUA_OK != result){
// 		printf("taskManStr loading error %d\n\r", result);
}

void LuaInit(void){
	// luaToHostStruct.bChanged = FALSE;
	// luaToHostStruct.len = 0;
	// luaToHostStruct.pMutex = OSSemCreate(1);
	
    L = luaL_newstate();
    luaL_openlibs(L); 
    luaL_newlib(L, RichonFunc);
    lua_setglobal(L, "richon");   
    
//	luaopen_bit32(L);	//LUA位运算支持库
	// luaL_newlib(L, BitFunc);
	// lua_setglobal(L, "Convert");
	
    LoadPlatform(L);
}

void Task_LuaVM(void * parameter){
    // LuaInit();
	// while (true){
	while (true){
		vTaskDelay(1);
	}

}



// }