#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletWorldImporter/btBulletWorldImporter.h>
#include "Motor3D.h"
#include <iostream>
#include <vector>
#include <stdio.h>

class PhysicsEngine
{
    public:
        //Instance
        static PhysicsEngine* Instance();
        void initPhysics(int);
        //Creates
        void createGround(float, float, float);
        void createPlayer(float, float, float);
                            //   x      y      z   size_x  size_y size_z  mass
        void createShape(float, float, float, float, float, float, float); // los tres primero float son las coordenadas x y z, los 3 siguientes floats son el tamaño del RigidBody
        void createDoor(float, float, float, float, float, float, float, int);
        void createExistingCapsule(float, float, float, btScalar,btScalar,float , btRigidBody*&, btDefaultMotionState*&, btCollisionShape*&);
        void createExistingShape(float, float, float, float, float, float, float, btRigidBody*&, btDefaultMotionState*&, btCollisionShape*&);
        void reposition(float, float, float, btRigidBody*&, btDefaultMotionState*&);
        void activateRigidBody(bool, btRigidBody*&);
        //Updates
        void updatePhysics(float);
        void updatePlayerPosition();
        //Inits
        void initMovePlayer();
        //Setters
        void setMovePlayer();
        void activeDebugDrawer();
        //Moves
        void movePlayerLeft();
        void movePlayerRight();
        void movePlayerForward();
        void movePlayerBackward();
        void moveRigidBody(btRigidBody*&, btVector3);
        void moveEnemy(btRigidBody*&, float, float, float);
        void moveEnemyLinear(btRigidBody*&, bool&, float, float, float&, float&, float&);
        void setPlayerPosition(float, float, float);
        void moveDragBox(btRigidBody*&, float&, float&, float&);
        //Clears
        void clearRigidBody(btRigidBody*&);
        void clearKinematic();
        //vector3df getRotationDegrees(irr::core::matrix4);
        //vector3df getTranslation(irr::core::matrix4);
        //

    protected:
        //Ctor & Dtor
        PhysicsEngine();
        virtual ~PhysicsEngine();
    private:        
        //Variables
        btDiscreteDynamicsWorld* m_dynamicsWorld;
        btDefaultCollisionConfiguration* m_collisionConfiguration;
       // static btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
        btBroadphaseInterface*	m_overlappingPairCache;
        btCollisionDispatcher*	m_dispatcher;
        btConstraintSolver*	m_constraintSolver;

        btAxisSweep3* sweepBP;
        btVector3 walkDirection;
        btScalar walkSpeed;
        btVector3 upDir;
        btVector3 forwardDir;
        class btBulletWorldImporter* fileLoader; //para cargar el mapa

        //Variables del player//
        btKinematicCharacterController* m_character;
        btPairCachingGhostObject* m_ghostObject;
        btConvexShape* capsule;

        btGhostPairCallback* playercallback;
        btGhostPairCallback* worldcallback;

        bool block_p_z;
        bool block_p_x;

        btScalar* scalar;
        btScalar* enemy_scalar;
        btScalar rota;

        //Debug

        ADebug *bulletDebugugger;
        bool isDebuggin;
};  

#endif // PHYSICSENGINE_H