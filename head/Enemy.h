#ifndef Enemy_H
#define Enemy_H

#include "NPC.h"
#include "Vector3f.h"
#include "Path.h"
#include "SMemory.h"
#include "Sensor.h"
#include "PhysicsEngine.h"

class Enemy: public NPC
{
    public:
        
        virtual ~Enemy(){}

        virtual void recalculatePath() = 0;
        virtual void updateAnimation() = 0;

        //Main methods
        void updatePEntities();
        bool detectEntity(Vector3f);
        void seek(Vector3f);
        void Avoidance(Vector3f);
        void Arrive(int, int);
        void followTheLeader(int, int);
        void reduceLife(int);
        bool isDeath();
        void Die();

        //Getters
        float getVisionVectorX();
        float getVisionVectorY();
        float getVisionVectorZ();
        bool  getchaseando();
        bool  getDeath();
        int   getAttack();
        Path* getPath();
        SMemory* getMemory();
        int   getSalaActual();
        int getState();
        float getMass();
        int   getHeardRatio();
        int   getSightRatio();
        int getLife();

        //Setters
        void  setVisionVectorX(float);
        void  setVisionVectorY(float);
        void  setVisionVectorZ(float);
        void  setVisionVectorValues(float, float, float);
        void  setMaxLife();     
        void  setchaseando(bool);
        void  setAnimState(int);    
        void  setSalaActual(int);
        void  setClock();
        void  setState(int);
        void  setMass(float);
        void  setDeactivated(bool);
        void  setPushVector(float,float,float);

    protected:
    	int   vida;
        int   maxlife;
        int   ataque;
        int   coins;
        bool  chaseando;
        int   salaActual;
        bool  death;
        int   w_clock;
        int   state;    // 0 - PATRULLA / 1 - CHASE / 2 - ATTACK
        int   h_r; //sight ratio
        int   s_r; //heard ratio

        Vector3f* vision_vector;
        Vector3f push_vector;
        Path* path;
        SMemory* memory;
        Sensor en_sensor;

        bool rleft;
        int  angle;

        //Physics
        btRigidBody* rigidBody;
    	btCollisionShape* bodyShape;
    	btDefaultMotionState* phy_state;
        float mass;
        bool deactivated;

        //animation
        bool  animation;
        int  anim_state;
        float a_time;
        float animationtime;
        int animationframe;
};

#endif // Enemy_H