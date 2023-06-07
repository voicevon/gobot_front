#pragma once

#include "Arduino.h"

#define LUA_USE_C89
#include "lua/lua.hpp"



class LuaWrapperBase {
	public:
		LuaWrapperBase(){};
		// virtual ~LuaWrapperBase(){};
		String Lua_dostring(const String *script);
		String Lua_dostring(const char *script);
		// void Begin(const char* filename);
		void Begin();
		// void SpinOnce();
		// virtual void comiler_vtable_error_even_subclass_has_defination_why ();  
		// virtual void comiler_has_no_error_why(){};   
		
	protected:
		// LuaWrapperBase(){};
		lua_State *_state;
		void _InitLua();
		void Lua_register(const String name, const lua_CFunction function);
		// virtual String _addConstants();

		bool _is_running = false;

	private:

};

