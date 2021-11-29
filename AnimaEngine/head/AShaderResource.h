#ifndef ASHADERRESOURCE_H
#define ASHADERRESOURCE_H

#include "AResource.h"
#include <string>
#include <fstream>
#include <sstream>

class AShaderResource: public AResource
{
    public:
        AShaderResource();
        virtual ~AShaderResource();
        bool loadResource(std::string);
		void draw();
		void use();
		//GETTER
		unsigned int getProgramID();
	private:
		void checkCompileErrors(unsigned int, std::string);
		unsigned int ID;
};

#endif // ASHADERRESOURCE_H