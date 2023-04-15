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
        void PrintFlat(const char* title);
        void CopyTo(char* destination);

        // const char &GetText = *__chars;  // For Arduino::chars[n]
        const char* GetChars = __chars;   // For std::cxx11::string

    private:
        char __chars[REPRAP_GCODE_MAX_SIZE];
        int __length;

};
