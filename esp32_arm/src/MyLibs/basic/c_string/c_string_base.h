#pragma once

// #define  REPRAP_GCODE_MAX_SIZE  38

#define C_STRING_OK  1
#define C_STRING_ERR_OVER_SIZE -2
#define C_STRING_ERR_ -1

class  C_String_Base{
    public:
        C_String_Base(){};
        void Init(char* buffer, int buffer_size);
        int CopyFrom(const char* bytes, int length);
        int CopyFrom(const char* bytes);
        void CopyTo(char* destination);
        void Concat(const char* bytes);
        bool IsEqualTo(const char* chars);
        bool IsPrefix(const char* chars);
        void RemovePrefix(const char prefix_ender);

        const char* c_str(){return __text;};
        int Find_MismachLocation(const void* b, int length);
        int GetBufferSize(){return __buffer_size;};   //TODO:  remove this
        int GetStringLength(){return __text_length;};
        void PrintFlat(const char* title);
        

        // const char &GetText = *__text;  // For Arduino::chars[n]
        // const char* GetChars = __text;   // For std::cxx11::string
    protected:
        // C_String_Base(int buffer_size);
        void _CopyTo(char* destination, int remove_prefix_bytes_length);
        C_String_Base(int buffer_size, char* buffer);

    private:
        char* __text;
        int __buffer_size;
        int __text_length;

};


