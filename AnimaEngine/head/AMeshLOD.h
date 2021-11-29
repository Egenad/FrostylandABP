#ifndef AMESHLOD_H
#define AMESHLOD_H

#include "AEntity.h"
#include "AMeshLODResource.h"
#include "ATextureResource.h"
#include "AMaterialResource.h"
#include <string>
#include <vector>

class AMeshLOD: public AEntity
{
    public:
        AMeshLOD();
        virtual ~AMeshLOD();
        bool loadResource(std::string);
        void beginDraw();
        void endDraw();
       	//----------GETTER----------
        GLuint getProgramId() const;
        /*AMeshResource* getMeshResource() const;
        ATextureResource* getTextureResource() const;
        AMaterialResource* getMaterialResource() const;*/
        int getActive();
        //----------SETTER----------
        void setProgramId(GLuint);
        void setMeshLODResource(AMeshLODResource*);
        void setMeshLODTexture(int, ATextureResource*);
        void setTextureResource(ATextureResource*);
        void setMaterialResource(AMaterialResource*);
        void setMeshSilhouette(bool);
        void setActive(int);
    private:
    	AMeshLODResource* meshes;
    	//ATextureResource texture; por facilidad esto va en el tresourcemesh
    	GLuint programID;
        bool silhouette;
        int active;
};

#endif // AMESH_H