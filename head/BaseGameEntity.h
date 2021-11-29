#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

enum EntityType{ 
	P_SOLDIER_ENTITY,
    P_GENERAL_ENTITY,
    P_KPONGO_ENTITY,
    FOX_ENTITY,
    S_NORMAL_ENTITY,
    S_QUEST_ENTITY,
    QUEEN_ENTITY,
	NORMAL_DOOR_ENTITY,
    NORMAL2_DOOR_ENTITY,
    NORMAL3_DOOR_ENTITY,
    JAIL_DOOR_ENTITY,
    LEVEL_DOOR_ENTITY,
    BOSS_DOOR_ENTITY,
	PLAYER_ENTITY,
    CHEST_FOOD_ICE_ENTITY,
    CHEST_FOOD_FISH_ENTITY,
    CHEST_FOOD_MEAT_ENTITY,
    CHEST_FOOD_BERRY_ENTITY,
    CHEST_NORMAL_KEY_ENTITY,
    CHEST_LEVEL_KEY_ENTITY,
    CHEST_JAIL_KEY_ENTITY,
    CHEST_BOSS_KEY_ENTITY,
    CHEST_SWORD_ENTITY,
    CHEST_HAMMER_ENTITY,
    CHEST_SLINGSHOT_ENTITY,
    PRESSED_INTERACTIBLE,
    DRAGGABLE_INTERACTIBLE,
    BUTTON_INTERACTIBLE,
    CHEST_INTERACTIBLE,
    TRAP_ENTITY,
    PLATE_1_2_INTERACTIBLE,
    SOLID_ROOM,
    HOLLOW_ROOM,
    TRAP_ROOM,
    BOSS_ROOM,
    PLATFORM_ENTITY,
    TRIGGER_FALL,
    TRIGGER_NENEMIES,
    TRIGGER_ENEMIES,
    TRIGGER_CAMERA,
    TRIGGER_DIALOGO,
    TRIGGER_LEVEL,
    TRIGGER_KPONGO,
    TRIGGER_DOOR,
    DECORATION_TABLE,
    DECORATION_TORCH,
    DECORATION_BARREL,
    DECORATION_BOX,
    DECORATION_BED,
    DECORATION_CHAIR,
    DECORATION_CHAIN,
    DECORATION_SOLDIER,
    DECORATION_FLAG,
    DECORATION_STATUE1,
    DECORATION_STATUE2,
    DECORATION_COLUMN,
    DECORATION_THRONE,
    DECORATION_PAINTING,
    DECORATION_SWORDDECO,
    DECORATION_LANCE
};

struct EntityFlag{
	int id;
    float p_x;
    float p_y;
    float p_z;
	EntityType type;
};

class BaseGameEntity
{   
    public:
        BaseGameEntity();
        virtual ~BaseGameEntity();
        virtual void updateEntity()=0;
        int getID()const{return m_ID;}
        void setType(EntityType t){type = t;}
        EntityType getType(){return type;}
        float getX();
        float getY();
        float getZ();
        void setX(float);
        void setY(float);
        void setZ(float);
    protected:
        float ps_x;
        float ps_y;
        float ps_z;
    private:
        int                     m_ID;
        static int              m_nextValidID;
        void setID();
        EntityType type;
};

#endif // BASEGAMEENTITY_H
