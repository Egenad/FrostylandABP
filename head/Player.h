#ifndef PLAYER_H
#define PLAYER_H

#include "Node.h"
#include "Wearable.h"
#include "Globals.h"
#include "BaseGameEntity.h"
#include "Vector3f.h"
#include "Vector2f.h"
#include "DragBox.h"
#include "PhysicsEngine.h"
#include "Camera.h"

class Player : public BaseGameEntity
{
    public:

        //Instance
        static Player* Instance();

        //Update
        void updateEntity();
        void initPlayer();
        //Getters
        m3D::Node& getNode();
        m3D::Node& getShadow();
        //float getRadius();
        int   getLifes();
        int   getMoney();
        int   getKeys();
        bool  getBossKey();
        int   getSalaActual();
        bool  getLevelKey();
        bool  getJailKey();
        int*  getFood();
        int   getFoodType(int);
        bool* getWeapons();
        int   getLastSala();
        float getVelocity();
        float getVisionVectorX();
        float getVisionVectorY();
        float getVisionVectorZ();
        Camera* getCamera();
        bool getBlockMovLeft();
        bool getBlockMovRight();
        void setPositionEntity(Vector3f);
        bool getBlockMovUp();
        bool getBlockMovDown();
        DragBox* getInteractible();
        bool getMoving();
        bool  allAnglesofMovement();
        Vector2f getSize();
        Wearable* getWearable();
        btVector3& getWalkDirection();
        btScalar& getWalkSpeed();

        //Setters
        void setInteractTime(float);
        void setBossKey(bool);
        void setLevelKey(bool);
        void setJailKey(bool);
        void setPosition(Vector3f);
        void setVelocity(float);
        void setVisionVectorValues(float, float, float);
        void setMUp(bool);
        void setMDown(bool);
        void setMLeft(bool);
        void setMRight(bool);
        void setIsMoving(bool);
        void setDraggeable(DragBox*);
        void setLifes(int);
        void setCoins(int);
        void setKeys(int);
        void setMoney(int);
        void setWalkDirection(btVector3);
        void setWalkSpeed(btScalar);
        void setCState(int);
        void setPCameraState(int);
        void resetCameraPosition();
        void setPositionPhysic();
        void setPushed(bool);
        void setPushedVelocity(bool,bool);
        
        //Main methods
        void attack();
        void moveUp();
        void moveDown();
        void moveLeft();
        void updateCamera();
        void moveRight();
        void receiveDamage(int);
        void addMoney(int);
        void addFood(int);
        void addWeapon(int);
        void restMoney(int);
        void restKey();
        void addKey();
        void interactObject();
        void checkInteract();
        float calculateDistanceEnemy(int);
        float calculateAngleEnemy(int);
        void createPlayerKinematic();
        void heal(int);
        void eraseFood(int);
        void equipWeapon(int);
        void fullInventory();
        void updateAnimation();
        void playAudio(int);

        //Public destructor
        void clearInstance();
        
    protected:

        //Constructor and Destructor
        Player();
        virtual ~Player();

    private:
        
        //Characteristics
        int vida;
        int dinero;
        float velocidad;
        bool attacking;
        float interactTime;
        float height;
        float width;
        int salaActual;
        int lastSala;
        bool pushed;
        bool pushed_down;
        bool pushed_left;
        float push_time;

        //Movement
        bool m_up;
        bool m_down;
        bool m_right;
        bool m_left;
        bool ismoving;
        bool rleft;
        int  angle;
        Camera* pl_camera;

        Vector3f* vision_vector;

        //Timers
        float next_attack;

        //Objects
        int food[GLOBAL_FOOD_SIZE];
        DragBox* dragBox;
        //Weapons
        bool weapons[GLOBAL_WEAPONS_SIZE];
        Wearable* e_weapon;
     
        //Keys
        int  keys;
        bool boss_key;
        bool level_key;
        bool jail_key;
        
        //Object variables
        m3D::Node player;
        m3D::Node shadow;

        //Physics
        btVector3 walkDirection;
        btScalar walkSpeed;

        //animation
        float animationtime;
        int animationframe;
        bool resetanimation;
};

#endif // PLAYER_H
