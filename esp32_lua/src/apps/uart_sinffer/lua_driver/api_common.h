#pragma once
#include "lua/lua.h"

extern int LuaMqttPublish(lua_State* L);
extern int LuaMqttSubscribe(lua_State* L);
extern int LuaMqttReadFirstTopic(lua_State* L);
extern int LuaMqttReadPayload(lua_State* L);
