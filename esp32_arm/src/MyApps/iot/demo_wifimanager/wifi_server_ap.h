#pragma once
#include "WString.h"
#include "SPIFFS.h"

class WifiServerAp{
    public:
        static WifiServerAp& GetInstance(){
            return __instance;
        }
        static void StartApServer();
        static void initSPIFFS();
        static String readFile(fs::FS &fs, const char * path);
        static void writeFile(fs::FS &fs, const char * path, const char * message);
        static bool initWiFi();
        static void setup_callme(const char* file_name);
        static String GetSsid();
        static String GetPassword();

    private:
        static WifiServerAp __instance;
        // AsyncWebServer server(80);

};