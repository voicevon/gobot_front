#pragma once
#include "Arduino.h"
#pragma comment
#define LUA_USE_C89
#include "MyLibs/app_base/lua_wrapper/lua_wrapper_base.h"

class WaterDropper_LuaWrapper: public LuaWrapperBase {
	public:
		
	protected:
		void __Go_on_register() override;

	private:
};

