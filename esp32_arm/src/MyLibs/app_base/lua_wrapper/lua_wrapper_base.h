#pragma once

#include "Arduino.h"

#define LUA_USE_C89
#include "lua/lua.hpp"
#include "SPIFFS.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#include "esp_spiffs.h"


class LuaWrapperBase {
	public:
		String Lua_dostring(const String *script);
		void Begin(const char* filename);
		void SpinOnce();

	protected:
		LuaWrapperBase(){};
		lua_State *_state;
		void _InitLua();
		void Lua_register(const String name, const lua_CFunction function);
		virtual String _addConstants();
		// char _filename[20];
		bool _is_running = false;
		FsFilename __lua_filename;
		fs::File* __lua_file = nullptr;
		// fs::File* __lua_file__;

	private:

};

