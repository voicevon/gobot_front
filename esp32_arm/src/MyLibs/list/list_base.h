#pragma once

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
        bool __AddItem(ListItem* item){__all_items[__index]=item;__index++; return true;};  //todo:  check overflow
        ListItem* _GetItem(int index){return (ListItem*)(__all_items[index]);};
        void _Init(ListItem** list, int items_count){__all_items=list,__items_count=items_count;};

    private:
        ListItem** __all_items;
        int __items_count;
        int __index;
        // int __item_size;   // unit in byte.

};


