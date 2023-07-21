#pragma once
#include "WString.h"
#include "LittleFS.h"
// #include "SPIFFS.h"
#include "base/ap_webserver_diction_base.h"


class WebServerStarter{
    public:
        enum EnumState{
            IDLE,
            CONNECTING_TO_ROUTER,
            AS_AP,
            // AS_STA,
            ADMIN_DONE,
            // RESTART,
            // RETURN,
        };

        // static EnumState state;
        static WebServerStarter& GetInstance(){
            return __instance;
        }
        static void Begin(ApWebserver_DictionBase* web_configurator_diction);
        static const char* GetSsid();
        static const char* GetPassword();
        static const char* GetConfig(const char* key);

    private:
        static WebServerStarter __instance;
        static void __Wifi_EnterAPMode();
        static void __StartLuaEditor();
        static bool __Connect_to_a_Router();
        static void __StartWebServer_forAP();

        static String processor_upload_file(const String& var);
        static void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
        static String humanReadableSize(const size_t bytes);
        static String listFiles(bool ishtml);

        static bool __statemachine_spinonce();

};