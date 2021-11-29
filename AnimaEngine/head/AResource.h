#ifndef ARESOURCE_H
#define ARESOURCE_H

#include <string>
#include <glew.h>
#include <gl.h>

class AResource
{
    public:
    	std::string getName(){ return name; }
    	void setName(std::string n){ name = n; }
        virtual bool loadResource(std::string) = 0;
		virtual void draw() = 0;
		virtual ~AResource(){};
    protected:
    	std::string name;
};

#endif // ARESOURCE_H