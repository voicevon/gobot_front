#pragma once

#include "von/utility/logger.h"

class ListItem{
    public:

    private:

};

class ListBase{ 
    public:
        int GetItemsCount(){return this->__list_size;};
        void PrintOUt(const char* title){
            Logger::Info(title);
            Logger::Print("__list_name", __list_name);
            Logger::Print("__list_size", __list_size);
            Logger::Print("__index", __index);
        };

    protected:
        void __AddItem(ListItem* item){
            if (__index >= __list_size){
                Logger::Error("ListBase::__AddItem()");
                Logger::Print("__index", __index);
                Logger::Print("__list_size", __list_size);
                Logger::Halt("List Item is Overflow!");
            }
            __all_items[__index] = item;
            __index++; 
            };  
        ListItem* _GetItem(int index){
            if(index >= __list_size){
                Logger::Error("ListBase::_GetItem()");
                Logger::Print("index", index);
                Logger::Halt("List Item is Outside");
            }
            return (ListItem*)(__all_items[index]);
            };

        void _Init(const char* list_name, ListItem** list, int items_count){
            __list_name = list_name;
            __index = 0;
            __all_items = list;
            __list_size = items_count;
            };

    private:
        ListItem** __all_items;
        const char* __list_name;
        int __list_size;
        int __index;

};


