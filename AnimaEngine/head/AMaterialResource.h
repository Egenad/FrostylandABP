#ifndef AMATERIALRESOURCE_H
#define AMATERIALRESOURCE_H

#include "AResource.h"

class AMaterialResource: public AResource
{
    public:
        AMaterialResource();
        virtual ~AMaterialResource();
        bool loadResource(std::string);
		void draw();
		void setProgramID(unsigned int);
	private:
		float *kambient, *kdiffuse, *kspecular; //these variables are pointers because they are a vec3 of floats
												//this way we can reserve memory with malloc and free it later
		float kglossiness;
		unsigned int progID;
};

#endif // AMATERIALRESOURCE_H