#pragma once
#include "WString.h"
#include "SPIFFS.h"


class WifiServerAp{
    public:
        static WifiServerAp& GetInstance(){
            return __instance;
        }
        /// @brief 
        /// @param html_filename 
        /// @param gpio_of_config_button: -1 to disable config_button.
        static void Begin(String html_filename, int8_t gpio_of_config_button);
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