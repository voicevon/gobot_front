#pragma once
#include <lua/lua.h>


extern int LuaUartRecv_Master(lua_State* L);
extern int LuaUartSend_Master(lua_State* L);

extern void Task_MasterUart(void * parameter);
