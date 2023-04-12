#pragma once

#define  REPRAP_GCODE_MAX_SIZE  38

class  GcodeText{
    public:
        GcodeText();
        GcodeText(const char* bytes);
        void ReConstruct(const char* bytes, int length);
        char bytes[REPRAP_GCODE_MAX_SIZE];

    private:
};
