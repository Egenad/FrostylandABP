#ifndef AMESH_H
#define AMESH_H

#include "AEntity.h"
#include "AMeshResource.h"
#include "ATextureResource.h"
#include "AMaterialResource.h"
#include <string>

class AMesh: public AEntity
{
    public:
        AMesh();
        virtual ~AMesh();
        AMesh(const AMesh&);
        void loadMesh(std::string);
        void beginDraw();
        void endDraw();
       	//----------GETTER----------
        GLuint getProgramId() const;
        AMeshResource* getMeshResource() const;
        ATextureResource* getTextureResource() const;
        AMaterialResource* getMaterialResource() const;
        //----------SETTER----------
        void setProgramId(GLuint);
        void setMeshResource(AMeshResource*);
        void setTextureResource(ATextureResource*);
        void setMaterialResource(AMaterialResource*);
        void setMeshSilhouette(bool);
    private:
    	AMeshResource* mesh;
    	//ATextureResource texture; por facilidad esto va en el tresourcemesh
    	GLuint programID;
        bool silhouette;
};

#endif // AMESH_H