#ifndef ESTADO_H
#define ESTADO_H

class Estado
{
    public:

    	enum StateType{
		    //STATES
		    GAME_OVER_STATE,
		    MENU_STATE,
		    DUNGEON_STATE,
            INVENTORY_STATE,
            DIALOG_STATE,
            ITEM_SELECTOR_STATE,
            PAUSE_MENU_STATE,
            OPTIONS_STATE
		};

        virtual void handleInput()=0;
        virtual void draw() = 0;
        virtual void update() = 0;
        virtual void clearState()=0;
        virtual void addEvents()=0;
    private:
    	StateType type;
};

#endif // ESTADO_H
