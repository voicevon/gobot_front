#include <Arduino.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "ltable.h"
#include <ESPAsyncWebServer.h>
#include "../main.h"
#include <FS.h>
#include <LittleFS.h>

extern HardwareSerial serial;
extern AsyncWebSocket webSocket;

lua_State *L = 0;

int luaDelay(lua_State *state);
int luaPrint(lua_State *state);
int luaWsPrint(lua_State *state);
int luaFlashLED(lua_State *l);

void initLUA();

static const struct luaL_Reg luaFunc[] = {
    {"delay", luaDelay},
    {"myprint", luaPrint},
    {"wsprint", luaWsPrint},
    {"flashLed", luaFlashLED},
    {NULL, NULL}};

void luaEngineTask(void *pvParam)
{
    initLUA();
    while (true)
    {
        vTaskDelay(8000);
    }
}

char buf[1024 * 10];

void initLUA()
{
    L = luaL_newstate();

    luaL_openlibs(L);

    if (LittleFS.begin())
    {
        File demo = LittleFS.open("/json.lua");
        size_t len = demo.size();

        memset(buf, 0, sizeof(buf));
        demo.readBytes(buf, len);

        lua_getglobal(L, "package");
        lua_getfield(L, -1, "preload");
        int result = luaL_loadstring(L, buf);

        lua_setfield(L, -2, "json");
    }

    luaL_newlib(L, luaFunc);
    lua_setglobal(L, "richon");

    serial.println("lua engine running");
    serial.flush();
}

int luaDelay(lua_State *state)
{
    int i;
    int ms;
    i = lua_gettop(state);
    if (i == 1)
    {
        if (lua_isinteger(state, 1))
        {
            ms = lua_tointeger(state, 1);
            vTaskDelay(ms); // 延时超过5秒会出现Task WatchDog got Trigged问题，所以超过5秒需要进行多次延时
        }
    }
    return 0;
}

int luaPrint(lua_State *state)
{
    int i;
    size_t tmp;
    const char *p;

    i = lua_gettop(state);

    if (lua_isstring(state, i))
    {
        p = lua_tolstring(state, -1, &tmp);
    }

    serial.println(p);
    serial.flush();
    return 0;
}

int luaWsPrint(lua_State *l)
{
    size_t len;
    const char *p;
    int i = lua_gettop(l);

    if (lua_isstring(l, i))
    {
        p = lua_tolstring(l, -1, &len);
    }

    webSocket.textAll(p);

    return 0;
}

int luaFlashLED(lua_State *l)
{
    int flag = 0;
    lua_gettop(l);
    if (lua_isinteger(l, 1))
    {
        flag = lua_tointeger(l, 1);
        if (flag == 1)
        {
            digitalWrite(RED_PIN, HIGH);
            digitalWrite(GREEN_PIN, HIGH);
        }
        else
        {
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, LOW);
        }
    }

    return 0;
}