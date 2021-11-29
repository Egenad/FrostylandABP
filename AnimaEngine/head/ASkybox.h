#ifndef ASKYBOX_H
#define ASKYBOX_H

#include "AEntity.h"

class ASkybox: public AEntity
{
    public:
        ASkybox();
        virtual ~ASkybox();
        ASkybox(const ASkybox&);

        void loadSkybox();
        void beginDraw(int);
        void beginDraw();
        void endDraw();
        
    private:
        GLuint textureID;

        //draw buffers
        GLuint VBO, VAO;

};

#endif // ASKYBOX_H