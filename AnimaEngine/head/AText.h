#ifndef ATEXT_H
#define ATEXT_H

#include "AEntity.h"
#include <map>

struct Character {
    GLuint TextureID;   //texture id
    glm::ivec2 Size;    //Size of glyph                                     
    glm::ivec2 Bearing;  //Offset from baseline to left/top of glyph -> |-|a
    GLuint Advance;    //Horizontal offset to advance to next glyph -> |a-|b
};

class AText: public AEntity
{
    public:
        AText(glm::vec3, std::string, float, glm::vec3);
        virtual ~AText();
        void beginDraw();
        void endDraw();
        void setText(std::string);
        void setPosition(glm::vec3);
    private:
        std::map<GLchar, Character> Characters;

        GLuint *VBO, VAO, EBO;

        std::string text;
        float scale;
        glm::vec3 color;
        glm::vec3 pos;
        GLuint tid;
        bool charge;
};

#endif // ATEXT_H