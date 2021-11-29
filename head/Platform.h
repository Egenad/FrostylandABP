#ifndef PLATFORM_H
#define PLATFORM_H

#include "Object.h"
#include "Path.h"
#include "PhysicsEngine.h"
#include "Vector3f.h"

class Platform: public Object
{
    public:
        Platform(float,float,float);
        virtual ~Platform();
        void updateEntity();
        void generatePath(float, float, float, float, bool);
        void seek(Vector3f);
    private:
    	Path* path;
    	float velocidadX;
    	float velocidadZ;
    	float maxVel;
        int mov;
        float med;
        float velx;
        float velz;
    	btRigidBody* rigidBody;
    	btCollisionShape* bodyShape;
    	btDefaultMotionState* state;
};

#endif // PLATE_H