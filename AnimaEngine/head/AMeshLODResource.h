#ifndef AMESHLODRESOURCE_H
#define AMESHLODRESOURCE_H

#include "AResource.h"
#include "AMeshResource.h"
#include <vector>

class AMeshLODResource: public AResource
{
    public:
        AMeshLODResource(int);
        virtual ~AMeshLODResource();
        void loadUniforms(float*,float*, float*, GLuint, int);
        bool loadResource(std::string);
		void draw();
		void draw(int);
		//GETTERS

		//SETERS
        void setTextureResource(int, ATextureResource*);
        void setMaterialResource(AMaterialResource*);
	private:
		std::vector<AMeshResource*> meshes;
};

#endif // AANIMATIONRESOURCE_H