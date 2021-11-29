#ifndef ITEM_SELECTOR_H
#define ITEM_SELECTOR_H

#include "Estado.h"

class Item_Selector : public Estado
{
    public:
        static Item_Selector* Instance();
        void handleInput();
        void draw();
        void update();
        void clearState();
        void addEvents();
        void selectorUp();
        void selectorDown();
        void selectOption();
        void changeState();
        void selectedItem(int);
    protected:
        Item_Selector();
        virtual ~Item_Selector();
    private:
        bool p_released;
        bool drawed;
        float then;
        int selector;
        int item;
};

#endif // ITEM_SELECTOR_H
