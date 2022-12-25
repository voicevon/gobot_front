

class MyJsonBase{
    public:
        void InitJson();
        void DictionBegin();
        void DictionEnd();
        void ListBegin();
        void ListEnd();
        virtual void AppendToJson(char* key, char* value);
        virtual void AppendToJson()

    protected:
        char* _buffer;
};