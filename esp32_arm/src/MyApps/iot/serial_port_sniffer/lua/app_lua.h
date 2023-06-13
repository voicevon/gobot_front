#pragma once
#include "Arduino.h"
#pragma comment
#define LUA_USE_C89
#include "MyLibs/app_base/lua_base/von_lua_base.h"

class SerialPortSniffer_LuaWrapper: public LuaBase {
	public:
		void _Go_on_register() override;


	protected:

	private:
};

