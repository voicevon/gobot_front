#pragma once

#include "diction_base.h"

class MyItem: public DictionItem{
    public:
        int test_id;

    private:
};

class MyDiction: public DictionBase{
    public:
        MyItem* GetMyItem(int index){
            return (MyItem*)(_GetItem(index));
        };
        void Init(int items_count){
            _Init(items_count, sizeof(MyItem));
        };

    private:
        
};

void test(){
    MyDiction::Instance()._Init(11,22);
    
    MyDiction::Instance().GetItem(2);
}