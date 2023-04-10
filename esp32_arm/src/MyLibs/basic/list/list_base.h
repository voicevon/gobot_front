#pragma once

#include "MyLibs/basic/logger.h"

class ListItem{
    public:

    private:

};

class ListBase{ 
    public:
        int GetItemsCount(){return this->__items_count;};
        
    protected:
        void __AddItem(ListItem* item){
            if (__index >= __items_count){
                Logger::Error("ListBase::__AddItem()");
                Logger::Print("__index", __index);
                Logger::Print("__items_count", __items_count);
                Logger::Halt("List Item is Overflow!");
            }
            __all_items[__index] = item;
            __index++; 
            };  
        ListItem* _GetItem(int index){
            if(index >= __items_count){
                Logger::Error("ListBase::_GetItem()");
                Logger::Print("index", index);
                Logger::Halt("List Item is Outside");
            }
            return (ListItem*)(__all_items[index]);
            };

        void _Init(ListItem** list, int items_count){
            __index = 0;
            __all_items = list;
            __items_count = items_count;
            };

    private:
        ListItem** __all_items;
        int __items_count;
        int __index;

};


