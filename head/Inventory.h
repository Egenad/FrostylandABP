#ifndef INVENTORY_H
#define INVENTORY_H

#include "Estado.h"
#include "Globals.h"
#include "Vector2f.h"

#define M(a,b)  Vector2f((a), (b))
#define M2(a,b,c,d,e,f) M(a,b), M(c,d), M(e,f)

class Inventory : public Estado
{
    public:
        static Inventory* Instance();
        void handleInput();
        void draw();
        void drawImages();
        void update();
        void clearState();
        void changeState();
        void addEvents();
        void inventoryUp();
        void inventoryDown();
        void inventoryRight();
        void inventoryLeft();
        void inventorySelect();
        int  getItemSelected();
        void setEikaState(int);
        const Vector2f getPosition2d(int);
    protected:
        Inventory();
        virtual ~Inventory();
    private:
        bool e_released;
        bool p_released;
        bool drawed;
        float then;
        int selector;
        int eikaState;
        const Vector2f box[GLOBAL_INVENTORY_SIZE]={
            M2(-0.46,0.365,     -0.365,0.365,   -0.27,0.365),

            M2(-0.46,0.196,     -0.365,0.196,   -0.27,0.196), M2(-0.175,0.196,   -0.084,0.196,     0.01,0.196),
            M2(-0.46,0.052,     -0.365,0.052,   -0.27,0.052), M2(-0.175,0.052,   -0.084,0.052,     0.01,0.052),
            M2(-0.46,-0.09,     -0.365,-0.09,   -0.27,-0.09), M2(-0.175,-0.09,   -0.084,-0.09,     0.01,-0.09),
            M2(-0.46,-0.24,     -0.365,-0.24,   -0.27,-0.24), M2(-0.175,-0.24,   -0.084,-0.24,     0.01,-0.24),

            M2(-0.46,-0.41,     -0.365,-0.41,   -0.27,-0.41), M2(-0.175,-0.41,   -0.084,-0.41,     0.01,-0.41)
        };
};

#undef M
#undef M2

#endif // Inventory_H
