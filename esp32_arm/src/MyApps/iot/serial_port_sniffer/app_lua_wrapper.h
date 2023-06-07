#pragma once
#include "Arduino.h"
#pragma comment
#define LUA_USE_C89
#include "MyLibs/app_base/lua_wrapper/lua_wrapper_base.h"

class SerialPortSniffer_LuaWrapper: public LuaWrapperBase {
	public:
		void __Go_on_register() override;


	protected:
		String _addConstants() override;

	private:
		// lua_State *_state;
};

