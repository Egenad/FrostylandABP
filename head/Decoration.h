#pragma once
#ifndef DECORATION_H
#define DECORATION_H

#include "BaseGameEntity.h"
#include "PhysicsEngine.h"
#include "Node.h"
#include "SoundEvent.h"


class Decoration: public BaseGameEntity
{
    public:
        Decoration(float,float,float,EntityType,int);
        virtual ~Decoration();
        void updateEntity();
        m3D::Node& getNode();
        void drawDecoration();
        void playAudio(int);

    private:
        int angle, sprite;
        float timer;
        m3D::Node object;
        btRigidBody* rigidBody;
        btCollisionShape* bodyShape;
        btDefaultMotionState* state;
        SoundEvent* sound;

};

#endif // Chest_H