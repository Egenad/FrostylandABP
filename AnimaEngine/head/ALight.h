#ifndef ALIGHT_H
#define ALIGHT_H

#include "AEntity.h"

class ALight: public AEntity
{
    public:
        ALight();
        virtual ~ALight();
        void setIntensity(glm::vec3);
        glm::vec3 getIntensity() const;
        void beginDraw();
        void endDraw();
    private:
    	glm::vec3 intensity;
};

#endif // TLIGHT_H