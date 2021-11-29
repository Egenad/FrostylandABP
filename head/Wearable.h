#ifndef Wearable_H
#define Wearable_H

#include "Object.h"


class Wearable: public Object
{

public:
    virtual void playAudio(int) = 0;
    //Getters
    float getDamage(){
        return damage;
    }

    float getHitRange(){
        return hitRange;
    }

    float getAttackVel(){
        return attackVel;
    }
    float getAttackAng(){
        return attackAng;
    }
    m3D::Node& getBullet(){
        return bullet;
    } 

    char getType(){
        return type;
    } 

    //Setters
    void  setDamage(float da){
        damage = da;
    }

    void  setHitRange(float ra){
        hitRange = ra;
    }

    void  setAttackVel(float att){
        attackVel = att;
    }  

            
    protected:
        //Characteristics
        float damage;       //daño del arma
        float hitRange;     //rango del arma
        float attackVel;    //velocidad de ataque
        float attackAng;    //angulo de ataque
        char  type;
        m3D::Node bullet;

};

#endif // Wearable_H