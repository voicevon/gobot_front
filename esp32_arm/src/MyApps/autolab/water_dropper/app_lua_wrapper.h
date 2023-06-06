#pragma once
#include "Arduino.h"
#pragma comment
#define LUA_USE_C89
#include "MyLibs/app_base/lua_wrapper/lua_wrapper_base.h"

class WaterDropper_LuaWrapper: public LuaWrapperBase {
	public:
		WaterDropper_LuaWrapper() : LuaWrapperBase(){};
		// void comiler_vtable_error_even_subclass_has_defination_why() override {};

	protected:
		// String _addConstants() override {return "";};

	private:
};

