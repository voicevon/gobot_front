#pragma once

class DictionItem{
    public:

    private:

};


class DictionBase{  // HomerCollects? HomerDiction? 
    public:

        DictionItem* GetItem(int index);
        void _Init(DictionItem* the_array, int items_count, int item_size){this->__items_count=items_count;this->__item_size=item_size; __all_items=the_array; };
        int GetItemsCount(){return this->__items_count;};
    protected:
        DictionItem* _GetItem(int index){return (DictionItem*)(&__all_items[0] + __item_size*index);};

    private:
        DictionItem* __all_items;
        int __items_count;
        int __item_size;   // unit in byte.

};


