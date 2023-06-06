#pragma once
#include "WString.h"
#include "SPIFFS.h"
#include "base/web_configurator_diction_base.h"


class WebConfigurator{
    public:
        struct Config {
        String ssid;               // wifi ssid
        String wifipassword;       // wifi password
        int webserverporthttp;     // http port number for web admin
        };
        static WebConfigurator& GetInstance(){
            return __instance;
        }
        /// @brief 
        /// @param html_filename 
        /// @param gpio_of_config_button: -1 to disable config_button.
        static void Begin(WebConfigurator_DictionBase* web_configurator_diction);
        static const char* GetSsid();
        static const char* GetPassword();
        static const char* GetConfig(const char* key);

    private:
        static void __StartApServer();
        static void __StartLuaEditor();
        static bool __initWiFi();
        static WebConfigurator __instance;
        // String processor(const String& var);

        static String processor_upload_file(const String& var);
        static void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
        static void __InitApWebServer();
        static String humanReadableSize(const size_t bytes);
        static String listFiles(bool ishtml);

        // configuration structure


        // variables
};