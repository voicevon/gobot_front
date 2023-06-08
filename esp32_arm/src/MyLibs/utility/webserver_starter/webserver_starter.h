#pragma once
#include "WString.h"
#include "SPIFFS.h"
#include "base/ap_webserver_diction_base.h"


class WebConfigurator{
    public:
        enum EnumState{
            IDLE,
            CONNECTING_TO_ROUTER,
            AS_AP,
            AS_STA,
            // RESTART,
            // RETURN,
        };

        // static EnumState state;
        static WebConfigurator& GetInstance(){
            return __instance;
        }
        static void Begin(WebConfigurator_DictionBase* web_configurator_diction, bool webserver_on_ap_only);
        static const char* GetSsid();
        static const char* GetPassword();
        static const char* GetConfig(const char* key);

    private:
        static WebConfigurator __instance;
        static void __EnterWifiApMode();
        static void __StartLuaEditor();
        static bool __Connect_to_a_Router();
        static void __InitWebServer();

        static String processor_upload_file(const String& var);
        static void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
        static String humanReadableSize(const size_t bytes);
        static String listFiles(bool ishtml);

};