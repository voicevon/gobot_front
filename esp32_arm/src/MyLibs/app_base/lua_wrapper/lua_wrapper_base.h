#pragma once

#include "Arduino.h"
#define LUA_USE_C89
#include "lua/lua.hpp"
#include "MyLibs/utility/logger.h"
#include "MyLibs/mqtt/mqtt_subscriber_base.h"

class LuaWrapperBase: public MqttSubscriberBase{
	public:
		// String Lua_dostring(const String *script);
		String Lua_dostring(const char *script);
		void Begin();
		void LoadString(String* content);
		void Link_Mqtt(const char* mqtt_topic);

		
	protected:
		lua_State *_state;
		virtual void __Go_on_register();
		void _Lua_register(const String name, const lua_CFunction function);
		bool _is_running = false;

		void onGot_MqttMessage(const char* payload, uint16_t payload_len) override;

	private:

};

