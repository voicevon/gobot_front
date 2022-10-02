#pragma once

#include "diction_base.h"

class MyItem: public DictionItem{
    public:
        int test_id;

    private:
};

class MyDiction: public DictionBase{
    public:
        static MyDiction& Instance(){
            static MyDiction instance;
            return instance;
        }
        MyItem* GetMyItem(int index){
            return (MyItem*)(_GetItem(index));
        };
        void Init(MyItem* buffer, int items_count){
            _Init(buffer, items_count, sizeof(MyItem));
        };

    private:
        
};

void test(){
    MyItem buffer[20];
    MyDiction::Instance()._Init(buffer, 20,sizeof(MyItem));
    
    MyDiction::Instance().GetItem(2);
}