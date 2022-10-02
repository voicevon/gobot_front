#pragma once

#include "MyLibs/basic/logger.h"

class ListItem{
    public:

    private:

};

// Is this a List?   or diction?
class ListBase{  // HomerCollects? HomerDiction? 
    public:
        // ListItem* GetItem(int index);
        int GetItemsCount(){return this->__items_count;};
        
    protected:
        bool __AddItem(ListItem* item){
            Logger::Print("Index",__index);
            __all_items[__index] = item;
            // ListItem** addr = (ListItem**)(__all_items + __index * __SIZEOF_POINTER__);
            // *addr = item;
            Logger::Print("_add()",2);
            __index++; 
            Logger::Print("_add()",3);

            return true;
            };  //todo:  check overflow
        ListItem* _GetItem(int index){return (ListItem*)(__all_items[index]);};
        void _Init(ListItem** list, int items_count){
            Logger::Print("_init()",2);
            __index = 0;
            __all_items = list;
            __items_count = items_count;
            Logger::Print("_init()",3);
            };

    private:
        ListItem** __all_items;
        int __items_count;
        int __index;
        // int __item_size;   // unit in byte.

};


