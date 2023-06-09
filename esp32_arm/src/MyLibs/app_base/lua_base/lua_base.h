#pragma once

#include "Arduino.h"
#define LUA_USE_C89
#include "lua/lua.hpp"
#include "MyLibs/utility/logger.h"
#include "MyLibs/mqtt/mqtt_subscriber_base.h"

class LuaWrapperBase: public MqttSubscriberBase{
	public:
		String Lua_dostring(const char *script);
		void Begin();
		void Link_Mqtt_for_Test(const char* mqtt_topic);
		
	protected:
		virtual void _Go_on_register();
		lua_State *_lua_state;

	private:
		void onGot_MqttMessage(const char* payload, uint16_t payload_len) override;
		bool __is_running = false;

};

