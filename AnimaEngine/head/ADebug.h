#ifndef ADEBUG_H
#define ADEBUG_H

#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "AResourceManager.h"
#include <btBulletDynamicsCommon.h>

class ADebug : public btIDebugDraw
{
public:
     struct LINE {
        glm::vec3 from;
        glm::vec3 to;

        LINE(glm::vec3 f, glm::vec3 t) {
            from = f;
            to = t;
        }
    };

    struct COLOR {
        glm::vec3 col;

        COLOR(glm::vec3 c) {
            col = c;
        }
    };

    ADebug(void): m_debugMode(1), level(1) {}
    ~ADebug(void) {}

    void getLevel(int);
    void drawLines();
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}
    virtual void drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha) {}

    void reportErrorWarning(const char * warningString) { std::cout << "Physics debugger warning: " << warningString << std::endl; }
    
    virtual void draw3dText(const btVector3& location,const char* textString) {}
    virtual void setDebugMode(int debugMode) { m_debugMode = debugMode; }    
    virtual int getDebugMode() const { return m_debugMode; }

    std::vector<LINE> & GetLines() { return lines; }
    std::vector<COLOR> & GetColors() { return colors; }

private:
    std::vector<LINE> lines;
    std::vector<COLOR> colors;

	GLuint *VBO, VAO;
    
    int m_debugMode;
    int level;
};

#endif


