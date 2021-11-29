#ifndef ABILLBOARD_H
#define ABILLBOARD_H

#include "AEntity.h"
#include "ATextureResource.h"

class ABillboard: public AEntity
{
    public:
        ABillboard(glm::vec3, ATextureResource*, glm::vec3);
        virtual ~ABillboard();
        void beginDraw();
        void endDraw();
        void setPosition(glm::vec3);
        bool getActive();
        void setTexture(ATextureResource*);
        void setActive(bool);
    private:
    	ATextureResource* image;
        float width, height;
    	GLfloat* verts;
	  	GLuint *VBO, VAO, EBO;
        bool active;
};

#endif // ABILLBOARD_H