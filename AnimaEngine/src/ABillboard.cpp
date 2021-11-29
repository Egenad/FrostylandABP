#include "ABillboard.h"

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

ABillboard::ABillboard(glm::vec3 p, ATextureResource* i, glm::vec3 s){
    image = i;
    active = true;
    
    //scale dimensions
    width = image->getWidth() * s.x;
    height = image->getHeight() * s.y;


    //generate the buffers
    VBO = (unsigned int*)malloc(sizeof(unsigned int) * 2);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 3, NULL, GL_STATIC_DRAW);

    //vertex
    setPosition(p);

    //textures
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, tex, GL_STATIC_DRAW);

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * 3, ind, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

ABillboard::~ABillboard(){
    image = nullptr;

    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    free(VBO);
}

void ABillboard::setPosition(glm::vec3 p){
    //this method help us to move the billboard trough the screen during the program execution
    
    //vertex
    verts = (float *)malloc(sizeof(float) * 4 * 3);
    verts[0] = p.x+width;       verts[1] = p.y+height;          verts[2] = p.z;
    verts[3] = p.x+width;       verts[4]  = p.y;                verts[5]  = p.z;
    verts[6] = p.x;             verts[7]  = p.y;                verts[8]  = p.z;
    verts[9] = p.x;             verts[10]  = p.y+height;        verts[11]  = p.z;

    //vertex
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 4 * 3, verts);

    //FREE MEMORY
    free(verts);
}


void ABillboard::beginDraw(){
    if(active){
    	if(image){
    		image->draw();
    	}

        glBindVertexArray(VAO);

        //vertex
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3*sizeof(float), (GLubyte*)NULL);

        //textures
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 2*sizeof(float), (GLubyte*)NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void ABillboard::endDraw(){
	//vacio
}

void ABillboard::setTexture(ATextureResource* tr){
    image = tr;
}

void ABillboard::setActive(bool a){
    active = a;
}

bool ABillboard::getActive(){
    return active;
}