#pragma once

#include "Arduino.h"

#define LUA_USE_C89
#include "lua/lua.hpp"
#include "SPIFFS.h"


class LuaWrapperBase {
	public:
		LuaWrapperBase(){};
		String Lua_dostring(const String *script);
		void Begin(const char* filename);
		void SpinOnce();

	protected:
		void _InitLua();
		void Lua_register(const String name, const lua_CFunction function);
		lua_State *_state;
		virtual String _addConstants();
		char _filename[20];
		bool _is_running = false;

	private:

};

