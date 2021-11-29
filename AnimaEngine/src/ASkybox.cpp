#include "ASkybox.h"
#include <iostream>

#include "stb_image.h"

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

ASkybox::ASkybox(){
	
}

ASkybox::~ASkybox(){
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

ASkybox::ASkybox(const ASkybox& as){

}

// https://learnopengl.com/Advanced-OpenGL/Cubemaps
// https://github.com/SonarSystems/Modern-OpenGL-Tutorials/tree/master/%5BADVANCED%20OPENGL%5D/%5B17%5D%20Cubemap:Skybox
void ASkybox::loadSkybox(){

    for (int i = 0; i < 108; i++)
    {
        skyboxVertices[i] = skyboxVertices[i]*1000;

    }

    // Setup skybox VAO
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( skyboxVertices ), &skyboxVertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glBindVertexArray(0);

    //Texture
    std::vector<std::string> faces
    {
        "../assets/textures/skybox/front.tga",
        "../assets/textures/skybox/back.tga",        
        "../assets/textures/skybox/bottom.tga",
        "../assets/textures/skybox/top.tga",
        "../assets/textures/skybox/right.tga",
        "../assets/textures/skybox/left.tga",  
    };

	//Load Cubemap
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    unsigned char *data;

    for (GLuint i = 0; i < faces.size(); i++)
    {
        //flip vertycally the image
        stbi_set_flip_vertically_on_load(true);

        data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    //stop flipping vertycally the image
    stbi_set_flip_vertically_on_load(false);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture( GL_TEXTURE_CUBE_MAP, 0);

    data = nullptr;

}

void ASkybox::beginDraw(int sid){

	//pass the matrices to the shader
	glm::mat4 v, p;
	v = inverse(viewMatrix());
	p = projectionMatrix();

   //pass the view matrix to the shader
    GLuint loc = glGetUniformLocation(sid, "ViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &v[0][0]);

    //and the projection matrix
    loc = glGetUniformLocation(sid, "ProjectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &p[0][0]);


    glBindVertexArray( VAO );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );

}
void ASkybox::beginDraw(){

}

void ASkybox::endDraw(){

}