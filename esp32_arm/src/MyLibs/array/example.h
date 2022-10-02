#pragma once

#include "diction_base.h"

class MyItem: public ArrayItem{
    public:
        int test_id;

    private:
};

class MyArray: public ArrayBase{
    public:
        static MyArray& Instance(){
            static MyArray instance;
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
    MyArray::Instance()._Init(buffer, 20,sizeof(MyItem));
    
    MyArray::Instance().GetItem(2);
}