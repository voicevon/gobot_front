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
        static void StartApServer();
        // static void initSPIFFS();
        static bool initWiFi();
        static WebConfigurator __instance;


};