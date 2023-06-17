#include <Arduino.h>
#include "lua.hpp"
#include "lauxlib.h"
#include "AsyncWebSocket.h"
/**
 * 测试LUA代码任务
 * 测试代码通过Web的runscript脚本从浏览器编辑器传入，在runscript中按照脚本大小申请缓冲并把指针保存到与luaDemoTask共享的指针变量中
 * luaDemoTask启动后即处于suspend状态，等待runscript接收到需要测试的脚本并保存到缓冲后由runscript激活运行共享指针指向的缓冲中的LUA代码
 * luaDemoTask运行完脚本后通过WebSocket发送测试脚本运行完成信息后返回到suspend状态
 *
 */

char *pLuaCodeBuf;
extern lua_State *L;
// extern HardwareSerial serial;
extern AsyncWebSocket webSocket;

void luaDemoTask(void *pParams)
{
    while (true)
    {
        vTaskSuspend(NULL); // 参数为NULL为挂起自身，否则参数是挂起任务的handle，挂起后必须由vTaskResume激活
        webSocket.textAll("正在执行下发的测试脚本......");
        int result = luaL_dostring(L, (const char *)pLuaCodeBuf);

        if (result != 0)
        {
            const char *str = lua_tostring(L, -1); // Task WDT got Trigger应该是在这里激发的
            char temp[128];
            memset(temp, 0, sizeof(temp));
            Serial.printf("代码错误，原因：%s\n", str);
            webSocket.textAll(temp);
        }
        webSocket.textAll("测试脚本执行完成");
    }
}