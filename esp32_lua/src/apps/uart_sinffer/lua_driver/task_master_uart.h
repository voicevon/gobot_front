#pragma once
#include <lua/lua.h>


extern int LuaUartRecv(lua_State* L);
extern int LuaUartSend(lua_State* L);
// extern "C" {

// extern void Task_MasterUart(void * parameter);

// }