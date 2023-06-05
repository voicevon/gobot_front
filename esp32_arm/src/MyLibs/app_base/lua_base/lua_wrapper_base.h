#pragma once

#include "Arduino.h"

#define LUA_USE_C89
#include "lua/lua.hpp"

class LuaWrapperBase {
	public:
		LuaWrapperBase(){};
		String Lua_dostring(const String *script);
		void Begin();

	protected:
		void _InitLua();
		void Lua_register(const String name, const lua_CFunction function);
		lua_State *_state;
		virtual String _addConstants();

	private:

};

