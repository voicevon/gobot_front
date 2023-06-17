```
#include <Arduino.h>
// #include <FreeRTOS.h>
#include <freertos/FreeRTOS.h>
#include "lua/lua.hpp"

#include "lua/lualib.h"

#include "lauxlib.h"
#include "ltable.h"


void initLUA();
static int FlashLED(lua_State* l);
int LuaDelay(lua_State* l);

lua_State* L = 0;

extern HardwareSerial mySerial;

#define LED_PIN 2

static const struct luaL_Reg RichonFunc[] =
{
    {"delay"   , 	LuaDelay},
	{"flashLED",	FlashLED},
    {0, 0}
};

void luaEngineTask(void *pvParameters){
    initLUA();
    
    mySerial.println("LUA Engine Starting");

    for(;;){
        luaL_dostring(L, "richon.flashLED(0)");
        luaL_dostring(L, "richon.delay(1000)");
        luaL_dostring(L, "richon.flashLED(1)");
        luaL_dostring(L, "richon.delay(1000)");
    }
}

void initLUA(){
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_newlib(L, RichonFunc);
    lua_setglobal(L, "richon");
}

static int FlashLED(lua_State* l){
    int flag = 0;
    lua_gettop(l);
    if(lua_isinteger(l, 1)){
        flag = lua_tointeger(l, 1);
        if(flag == 1){
            digitalWrite(LED_PIN, HIGH);
        }else{
            digitalWrite(LED_PIN, LOW);
        }
    }

  return 0;
}

int LuaDelay(lua_State* l){
    int i;
    int ms;
    i = lua_gettop(l);
    if(i == 1)
    {
        if(lua_isinteger(l, 1))
        {
            ms = lua_tointeger(l, 1);
            delay(ms);
        }            
    }

    return 0;
}

```