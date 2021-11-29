#include "ADebug.h"
#include <gl.h>

void ADebug::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{

    int errorx,errory,errorz;

    if(level==1){
        errorx=0;
        errory=-100;
        errorz=0;
    }
    else if(level==2){
        errorx=8;
        errory=-10;
        errorz=-250;
    }
    else if(level==3){
        errorx=580;
        errory=70;
        errorz=50;
    }
    else if(level==4){
        errorx=0;
        errory=100;
        errorz=300;
    }

    lines.push_back(LINE(glm::vec3(((from.getX()*-1)+errorx), ((from.getY()*-1)+errory), (from.getZ()+errorz)), glm::vec3(((to.getX()*-1)+errorx), ((to.getY()*-1)+errory), (to.getZ()+errorz))));
    colors.push_back(COLOR(glm::vec3(color.getX(), color.getY(), color.getZ())));
    //std::cout<< "COLOR: " << color.getX() << ", " << color.getY() << ", " << color.getZ() << std::endl;
    //std::cout<< "------------------------------------------------------------------------" << std::endl;
}

void ADebug::getLevel(int lvl){
    level = lvl;
}


void ADebug::drawLines()
{

    /*std::cout << lines[0].from.x << ", " << lines[0].from.y <<", " << lines[0].from.z <<std::endl;
    std::cout << lines[1].from.x << ", " << lines[1].from.y <<", " << lines[1].from.z <<std::endl;
    std::cout << "line size: " << lines.size() << std::endl;
    std::cout << "colors size: " << colors.size() <<std::endl;*/

	/*GLfloat colo[] {
		1, 1, 1,
		1, 1, 1
	};*/

	//Dibujamos LINES

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glLineWidth( 5 );

	VBO = (unsigned int *)malloc(sizeof(unsigned int) * 2);
	glGenBuffers(2,VBO);

    //LINES
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); //telling opengl that this is the object buffer array
	glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(LINE), &lines[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //COLORS
    /*glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(COLOR), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);*/

    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, lines.size() * 2);

    lines.clear();
}


