#ifndef ATRANSFORM_H
#define ATRANSFORM_H

#include "AEntity.h"

class ATransform: public AEntity
{
    public:
        ATransform();
        virtual ~ATransform();
        void identity();
        void load(glm::mat4);
        void traspose();
        void traslate(float,float,float);
        void rotate(float,float,float,float);
        void scale(float, float, float);
        glm::mat4 getMatrix();
        void beginDraw();
        void endDraw();
    private:
        glm::mat4 matrix;
};

#endif // ATRANSFORM_H