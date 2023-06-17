#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include "lua.hpp"
#include "lauxlib.h"
#include "ArduinoJson.h"

// extern HardwareSerial Serial;

extern AsyncWebServer webServer;
extern AsyncWebSocket webSocket;
extern TaskHandle_t luaDemoTaskHandle;
extern char *pLuaCodeBuf;

extern lua_State *L;

void handleWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                          void *arg, uint8_t *data, size_t len);
void handleWebSocketData(void *arg, uint8_t *data, size_t len);

void listDir(fs::FS &fs, const char *dirname, uint8_t levels, char *buf)
{
    size_t pos = 0;
    // Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        // Serial.println("- failed to open directory");
        fs.mkdir(dirname);
        return;
    }
    if (!root.isDirectory())
    {
        // Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.print(file.name());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);

            // Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);

            if (levels)
            {
                listDir(fs, file.name(), levels - 1, buf);
            }
        }
        else
        {
            // Serial.print("  FILE: ");
            // Serial.print(file.name());
            // Serial.print("  SIZE: ");

            const char *fileName = file.name();
            // sprintf(&buf[pos], fileName);
            memcpy(&buf[pos], fileName, strlen(fileName));
            buf[pos + strlen(fileName)] = ' ';
            pos += strlen(fileName) + 1;

            // Serial.print(file.size());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            // Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}

String index_templateProcessor(const String &var)
{
    // 网页预处理
    // 网页中每个占位符（形式如：%STATE%）都调用一次处理，所以每次处理仅处理一个占位符变量的赋值即可
    if (var == "STA-IP")
    {
        return WiFi.localIP().toString();
    }
    return String();
}

void WebServerTask(void *pvParam)
{
    while (true)
    {
        vTaskDelay(1000);
    }
}

void webServerInit()
{
    Serial.println("webServerInit");
    if (LittleFS.begin(true))
    {
	    // webServer.serveStatic("/", LittleFS, "/");
        // webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        //              {
        //                 printf("receive index.html request");
        //                 request->send(LittleFS, "/index.html", String(), false, index_templateProcessor); });
        // webServer.on("^[\\s\\S]*$", HTTP_GET, [](AsyncWebServerRequest *request)
        //              {
        //                 if(strcmp(request->url().c_str(), "/ws") == 0){
        //                     return;
        //                 }
        //                 request->send(LittleFS, request->url(), String(), true); });

        // 所有URL，排除/ws，该URL用于WebSocket
        webServer.on("^((?!/ws).)*$", HTTP_GET, [](AsyncWebServerRequest *request)
                     { 
                        if ((strcmp(request->url().c_str(), "/index.html") == 0) || (strcmp(request->url().c_str(), "/") == 0)) {
                               request->send(LittleFS, "/index.html", String(), false, index_templateProcessor);
                               return;
                            } else {
                                //查询配置，从/config/dev.conf中获得数据后返回到前端
                                if(strcmp(request->url().c_str(), "/getconfig") == 0){
                                    // Serial.println("getconfig is called");
                                    uint8_t buf[2048];
                                    memset(buf, 0, 2048);
                                    if (LittleFS.begin(true))
                                    {
                                        fs::File file = LittleFS.open("/config/dev.config", "r", false);
                                        file.seek(SeekSet);
                                        size_t size = file.size();

                                        if(size < 2048){
                                            file.read(buf, size);
                                            // Serial.printf("file content is: %s\n", buf);
                                        }else{
                                            Serial.printf("size larged %d \n", size);
                                        }

                                        file.close();
                                    }
                                    request->send(200, "text/plain", (const char*)buf);
                                    return;
                                };
                                if(strcmp(request->url().c_str(), "/listfile") == 0) {
                                    if(LittleFS.begin(true)) {
                                        char buf[1024];
                                        memset(buf, 0, sizeof(buf));
                                        listDir(LittleFS, "/lua", 0, buf);
                                        // Serial.printf("buf is: %s", buf);
                                        //保存到缓冲中发送

                                        request->send(200, "tex/plain", String(buf));
                                        return;
                                    }else{
                                        Serial.println("listfile fs begin failed");
                                        request->send(200, "text/plain", "listfile fs failed");
                                        return;
                                    }
                                }

                                if(strcmp(request->url().c_str(), "/downloadfile") == 0) {
                                    const char* fileName = request->getParam("filename")->value().c_str();

                                    char buf[64];
                                    memset(buf, 0, sizeof(buf));
                                    memcpy(buf, "/lua/", 5);
                                    memcpy(&buf[5], fileName, strlen(fileName));

                                    if(LittleFS.begin()) {
                                        File file = LittleFS.open(buf);
                                        size_t len = file.size();
                                        char buf[len + 1];
                                        memset(buf, 0, sizeof(buf));
                                        file.readBytes(buf, len);
                                        // Serial.printf("\ncontent is: %s \n", buf);
                                        request->send(200, "text/plain", String(buf));
                                        return;
                                    }else{
                                        Serial.println("listfile fs begin failed");
                                        request->send(200, "text/plain", "listfile fs failed");
                                        return;
                                    }
                                } 

                                request->send(LittleFS, request->url(), String(), true); 
                            } });
        // 设置配置调用，把获得的数据保存在/config/dev.conf文件中
        // 参考链接：https://github.com/me-no-dev/ESPAsyncWebServer#body-data-handling
        webServer.on(
            "/setconfig", HTTP_POST, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "this is from setconfig"); },
            [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {},
            [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
            {
                if (!index)
                {
                    Serial.printf("BodyStart: %u B\n", total);
                    Serial.printf("data len is: %u \n", len);
                }

                for (size_t i = 0; i < len; ++i)
                {
                    Serial.write(data[i]);
                }

                // 把data数据保存在/config/dev.conf中
                if (LittleFS.begin(true))
                {
                    fs::File file = LittleFS.open("/config/dev.config", "w", true);
                    file.seek(SeekSet);
                    for (size_t i = 0; i < len; ++i)
                    {
                        file.write(data[i]);
                    }
                    // Serial.println("write finished");
                    file.close();
                }
                else
                {
                    Serial.println("LittleFS.begin failed");
                }

                if (index + len == total)
                {
                    // Serial.printf("\nBodyEnd: %u B \n", total);
                }
            });

        webServer.on(
            "/upload", HTTP_POST, [](AsyncWebServerRequest *request)
            { request->send(200); },
            [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
            {
                static File targetFile;
                static unsigned long startTimer;
                const char *path = "/lua/";
                if (!index)
                {
                    char name[strlen(filename.c_str()) + 2];
                    memset(name, 0, strlen(filename.c_str()) + 2);
                    memcpy(name, path, strlen(path));
                    memcpy(&name[strlen(path)], filename.c_str(), strlen((filename.c_str())));
                    Serial.println(name);

                    LittleFS.remove(name); // 首先删除，然后通过添加的形式保存
                    targetFile = LittleFS.open(name, FILE_WRITE);
                    startTimer = millis();
                    const char *FILESIZE_HEADER{"FileSize"};

                    // Serial.printf("UPLOAD: Receiving: '%s'\n", filename.c_str());
                    // Serial.printf("UPLOAD: fileSize: %s\n", request->header(FILESIZE_HEADER));
                }

                targetFile.seek(index, SeekCur); // 需要解决追加写入的问题

                targetFile.write(data, len);
                // targetFile.print((char *)data);

                // Serial.printf("index is: %i\n", index);
                if (final)
                {
                    Serial.printf("UPLOAD: Done. Received %i bytes in %.2fs which is %.2f kB/s.\n",
                                  index + len,
                                  (millis() - startTimer) / 1000.0,
                                  1.0 * (index + len) / (millis() - startTimer));
                    Serial.printf("write size is: %i\n", index + len);
                    targetFile.close();
                }
            });

        webServer.on(
            "/uploadtext", HTTP_POST, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "get content len"); },
            [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {},
            [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
            {
                for (int i = 0; i < len; ++i)
                {
                    Serial.write(data[i]);
                }

                DynamicJsonDocument doc(len);
                deserializeJson(doc, (char *)data);
                const char *fileName = doc["filename"];
                const char *luaCode = doc["luacode"];

                if (LittleFS.begin())
                {
                    const char *path = "/lua/";
                    char buf[32];
                    memset(buf, 0, sizeof(buf));
                    memcpy(buf, path, strlen(path));
                    memcpy(&buf[strlen(path)], fileName, strlen(fileName));

                    File file = LittleFS.open(buf, "w", true);
                    file.write((uint8_t *)luaCode, strlen(luaCode));

                    file.close();
                }
                else
                {
                    Serial.println("uploadtext littleFS.begin failed");
                }

                Serial.flush();
                Serial.printf("len is: %d\n", len);
            });

        webServer.on(
            "/runscript", HTTP_POST, [](AsyncWebServerRequest *request)
            { request->send(200); },
            [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {},
            [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
            {
                // char buf[len + 1];
                // memset(buf, 0, sizeof(buf));

                // DynamicJsonDocument doc(len);
                // deSerializeJson(doc, (char *)data);
                // const char *pCode = doc["code"];

                // memcpy(buf, pCode, strlen(pCode));

                // int result = luaL_dostring(L, (const char *)buf);
                // if (result != 0)
                // {
                //     const char *str = lua_tostring(L, -1); // Task WDT got Trigger应该是在这里激发的
                //     char temp[128];
                //     memset(temp, 0, sizeof(temp));
                //     sprintf(temp, "代码错误，原因：%s", str);
                //     webSocket.textAll(temp);
                // }

                // 修改为在luaDemoTask中执行
                // 判断luaDemoTask是否执行完成
                eTaskState state = eTaskGetState(luaDemoTaskHandle);

                if (state == eSuspended)
                {
                    // 首先释放缓冲，避免上次的缓冲没有释放造成内存泄露，一定要在测试任务处于挂起状态是释放，否则可能会影响测试任务
                    free(pLuaCodeBuf);
                    
                    pLuaCodeBuf = (char *)malloc(len + 1);
                    memset(pLuaCodeBuf, 0, len + 1);
                    DynamicJsonDocument doc(len);
                    deserializeJson(doc, (char *)data);
                    const char *pCode = doc["code"];
                    memcpy(pLuaCodeBuf, pCode, strlen(pCode));

                    // 激活luaDemoTask任务
                    vTaskResume(luaDemoTaskHandle);
                }
                else
                {
                    webSocket.textAll("测试任务正在执行，无法进行本次测试");
                }
            });
        Serial.println("using LittleFS");
    }
    else
    {
        webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                     { request->send(200, "text/plain", "File System can not mount, using static page"); });
        webServer.on("^[\\s\\S]*$", HTTP_GET, [](AsyncWebServerRequest *request)
                     { request->send(200, "text/plain", "File System can not mount, using static page"); });

        Serial.println("LittleFS hasn't mount, using using static webpage");
    }

    webSocket.onEvent(handleWebSocketEvent);
    webServer.addHandler(&webSocket);
    webServer.begin(); // 启动WebServer服务
}

void handleWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                          void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        break;
    case WS_EVT_DISCONNECT:
        break;
    case WS_EVT_DATA:
        handleWebSocketData(arg, data, len);
        break;
    case WS_EVT_PONG:
        break;
    case WS_EVT_ERROR:
        Serial.println("WS_EVT_ERROR is occured");
        break;
    default:
        break;
    }
}

void handleWebSocketData(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    if (info->final && info->index == 0 && info->len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        Serial.printf("websocket receive data: %s\n", data);
        Serial.flush();
        luaL_dostring(L, (const char *)data);
    }
}