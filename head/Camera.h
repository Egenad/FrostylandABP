#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3f.h"

class Camera
{
    public:
        Camera(float);
        virtual ~Camera();
        void updateCamera(float, float, float);
        void setCameraState(int);
        int getCameraState();
        void setCameraPosition(float, float, float);
        float getX();
        float getZ();
        float getY();
    private:
    	int state;
    	float ps_x, ps_y, ps_z;
    	float last_z;
        float velx, velz;
};

#endif // CAMERA_H