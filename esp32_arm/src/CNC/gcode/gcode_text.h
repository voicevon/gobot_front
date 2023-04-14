#pragma once

#define  REPRAP_GCODE_MAX_SIZE  38

#define GCODE_TEXT_OK  1
#define GCODE_TEXT_ERR_OVER_SIZE -2
#define GCODE_TEXT_ERR_ -1

class  GcodeText{
    public:
        GcodeText();
        GcodeText(const char* bytes);
        int CopyFrom(const char* bytes);
        int CopyFrom(const char* bytes, int length);
        void PrintFlat();
        void CopyTo(char* destination);
        char* GetChars(){return this->__chars;};

    private:
        char __chars[REPRAP_GCODE_MAX_SIZE];
        int __length;

};
