#pragma once

#define  REPRAP_GCODE_MAX_SIZE  38

class  GcodeText{
    public:
        GcodeText();
        GcodeText(const char* bytes);
        char bytes[REPRAP_GCODE_MAX_SIZE];

    private:
};
