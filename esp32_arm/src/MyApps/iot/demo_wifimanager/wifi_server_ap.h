#pragma once
#include "WString.h"
#include "SPIFFS.h"


class WifiServerAp{
    public:
        static WifiServerAp& GetInstance(){
            return __instance;
        }
        static void setup_callme(String html_filename);
        static const char* GetSsid();
        static const char* GetPassword();
        static const char* GetConfig(const char* key);

    private:
        static void StartApServer();
        static void initSPIFFS();
        static String readFile(fs::FS &fs, const char * path);
        static void writeFile(fs::FS &fs, const char * path, const char * message);
        static bool initWiFi();
        static WifiServerAp __instance;
        // AsyncWebServer server(80);


};