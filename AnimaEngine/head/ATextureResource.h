#ifndef ATEXTURERESOURCE_H
#define ATEXTURERESOURCE_H

#include "AResource.h"

class ATextureResource: public AResource
{
    public:
        ATextureResource();
        virtual ~ATextureResource();
        bool loadResource(std::string);
		void draw();
		//GETTERS
		int getWidth();
		int getHeight();
	private:
		unsigned int tid; //id for the texture
		bool isActive;
		int width, height;
};

#endif // ATEXTURERESOURCE_H