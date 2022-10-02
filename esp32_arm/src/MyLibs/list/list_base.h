#pragma once

class ListItem{
    public:

    private:

};

// Is this a List?   or diction?
class ListBase{  // HomerCollects? HomerDiction? 
    public:

        ListItem* GetItem(int index);
        void _Init(int items_count){this->__items_count=items_count;};
        int GetItemsCount(){return this->__items_count;};
        void AddItem(ListItem* item){__all_items[1]=item;};
        
    protected:
        ListItem* _GetItem(int index){return (ListItem*)(__all_items[index]};

    private:
        ListItem** __all_items;
        int __items_count;
        // int __item_size;   // unit in byte.

};


