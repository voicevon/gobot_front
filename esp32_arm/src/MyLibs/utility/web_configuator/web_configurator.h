#pragma once
#include "WString.h"
#include "SPIFFS.h"
// #include "web_configurator_diction_base.h"
#include "base/web_configurator_diction_base.h"


class WebConfigurator{
    public:

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
        static bool __initWiFi();
        static WebConfigurator __instance;
        // String processor(const String& var);

        String processor_upload_file(const String& var);
        void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
        void configureWebServer();
        String humanReadableSize(const size_t bytes);
        String listFiles(bool ishtml);

        // configuration structure
        struct Config {
        String ssid;               // wifi ssid
        String wifipassword;       // wifi password
        int webserverporthttp;     // http port number for web admin
        };

        // variables
        Config config;                        // configuration
};