#ifndef ACAMERA_H
#define ACAMERA_H

#include "AEntity.h"

class ACamera: public AEntity
{
    public:
        ACamera(float, float, float, float, float, float);
        ACamera(float, float, float, float);
        virtual ~ACamera();
        void setPerspective(float, float, float, float);
        void setPerspective(float, float, float, float, float, float);
        void setParalell(float, float, float, float, float, float);
        void beginDraw();
        void endDraw();
    private:
    	bool isPerspective;
    	float near;
    	float far;
        float up;
        float down;
        float right;
        float left;
};

#endif // ACAMERA_H