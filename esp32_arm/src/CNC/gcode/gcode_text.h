#pragma once

#define  REPRAP_GCODE_MAX_SIZE  38

class  GcodeText{
    public:
        GcodeText();
        GcodeText(const char* bytes);
        int ReConstruct(const char* bytes);
        int ReConstruct(const char* bytes, int length);  // TODO: remove this
        char bytes[REPRAP_GCODE_MAX_SIZE];
        void PrintFlat();
    private:
};
