#include <ft2build.h>
#include FT_FREETYPE_H
#include "AText.h"

const unsigned int ind[] = { 
    0, 1, 2,
    0, 2, 3
};

const float tex[]{
    1.0, 0.0,
    1.0, 1.0,
    0.0,1.0,
    0.0,0.0
};

AText::AText(glm::vec3 position, std::string t, float s, glm::vec3 c){

    //********************************************************
    // NOTICE: The code to use the stb library to implement
    //              text font was taken from:
    //    https://learnopengl.com/In-Practice/Text-Rendering
    //********************************************************

    text = t;
    color = c;
    pos = position;
    scale = s;
    charge = true;

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft) != 0)
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "../assets/fonts/arial.ttf", 0, &face) != 0)
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load first 128 characters of ASCII set
    for(GLubyte c = 0; c < 128; c++){
        // Load character glyph 
        if(FT_Load_Char(face, c, FT_LOAD_RENDER) != 0){
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            exit(0);
        }

        // Generate texture
        glGenTextures(1, &tid);
        glBindTexture(GL_TEXTURE_2D, tid);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            tid,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // Configure VAO/VBO for texture quads

    VBO = (unsigned int*)malloc(sizeof(unsigned int) * 2);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3*sizeof(float), (GLubyte*)NULL);

    //textures
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 2*sizeof(float), (GLubyte*)NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * 3, NULL, GL_STATIC_DRAW);


    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

AText::~AText(){
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    free(VBO);
    glDeleteTextures(1, &tid);
}

void AText::beginDraw(){

    if(charge){

        GLfloat x = pos.x;

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = Characters[*c];

            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;

            GLfloat verts[4][3] = {
                { xpos+w,   ypos + h,   pos.z},            
                { xpos+w,   ypos,       pos.z},
                { xpos,     ypos,       pos.z},
                { xpos,     ypos + h,   pos.z},          
            };

            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 4 * 2, tex);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * 2 * 3, ind);

            // Render quad
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void AText::endDraw(){

}

void AText::setText(std::string t){
    text = t;

    if(t.compare("") == 0){
        charge = false;
    }else{
        charge = true;
    }
}

void AText::setPosition(glm::vec3 p){
    pos = p;
}