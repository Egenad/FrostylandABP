#ifndef AANIMATIONRESOURCE_H
#define AANIMATIONRESOURCE_H

#include "AResource.h"
#include "AMeshResource.h"
#include <vector>

class AAnimationResource: public AResource
{
    public:
        AAnimationResource(int);
        virtual ~AAnimationResource();
        AAnimationResource(const AAnimationResource&);
        void loadUniforms(float*,float*, float*, GLuint, int);
        bool loadResource(std::string);
		void draw();
		void draw(int);
		//GETTERS

		//SETERS
        void setTextureResource(ATextureResource*);
        void setMaterialResource(AMaterialResource*);
	private:
		std::vector<AMeshResource*> meshes;
};

#endif // AANIMATIONRESOURCE_H