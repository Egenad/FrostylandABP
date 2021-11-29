#include "AMeshResource.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <iostream>

AMeshResource::AMeshResource(){
	nFaces = 0;
	nVertex = 0;
	vertex = nullptr;
	vertexIndices = nullptr;
	material = nullptr;
	texture = nullptr;
}

AMeshResource::~AMeshResource(){
	glDeleteBuffers(4, VBO);
    glDeleteVertexArrays(1, &VAO);

    texture = nullptr;
    material = nullptr;

	if(VBO){
		free(VBO);
		VBO = nullptr;
	}
}

AMeshResource::AMeshResource(const AMeshResource& amr){
	nFaces = amr.getnFaces();
	*VBO = *amr.getVBO();
	VAO = amr.getVAO();
}

void AMeshResource::loadUniforms(float* m, float* v, float* p, GLuint pid){

	/*glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);*/

 	//pass the view matrix to the shader
    GLuint loc = glGetUniformLocation(pid, "ViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, v);

    //the model matrix
    loc = glGetUniformLocation(pid, "ModelMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, m);

    //and the projection matrix
    loc = glGetUniformLocation(pid, "ProjectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, p);
}

bool AMeshResource::loadResource(std::string file){

	
	bool success = false;

	//first asign the name to the resource
	setName(file);

	//then we read the file using assimp library and load the data in our variables
	Assimp::Importer i;
	const aiScene* obj = i.ReadFile(name, aiProcess_Triangulate | aiProcess_FlipUVs);
	/*aiProcces_Triangulate -> transform the faces to triangles
	  aiProcces_FlipUVs -> flips the texture in the Y axis*/

	if(obj!=nullptr){

		//the scene could be composed of various meshes, we iterate through each one to get the data
		long nMeshes = obj->mNumMeshes;

		if(nMeshes > 0){
			//save the current mesh in a structure
			aiMesh* mesh = obj->mMeshes[0];

			//add the actual mesh's faces to the total faces
			int meshFaces = mesh->mNumFaces;
			nFaces = nFaces + meshFaces;

			nVertex = mesh->mNumVertices;

			//generating vertex array object (contains VBO)
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			//-------------------------VERTEX----------------------------
			//reserve memory for the vertex array
			vertex = (float *)malloc(sizeof(float) * nVertex * 3);

			//copy the vertex data (position of each vertex) stored in the mesh to our structure (vector)
			memcpy(&vertex[0], mesh->mVertices, 3 * sizeof(float) * nVertex);

			//generating vertex buffer object (buffer wich contains objects)
			VBO = (unsigned int *)malloc(sizeof(unsigned int) * 4);
			glGenBuffers(4,VBO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); //telling opengl that this is the object buffer array
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertex * 3, vertex, GL_STATIC_DRAW);

			//-------------------------NORMALS---------------------------

			GLfloat* normals;

			if(mesh->HasNormals()){
				//reserve memory for the normals array
				normals = (float *)malloc(sizeof(float) * nVertex * 3);

				//copy the vertex data stored in the mesh to our structure
				memcpy(&normals[0], mesh->mNormals, 3 * sizeof(float) * nVertex);

				glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertex * 3, normals, GL_STATIC_DRAW);

			}

			//-------------------------TEXTURES--------------------------

			GLfloat* textures;

			if(mesh->HasTextureCoords(0)){
				
				textures = (float *)malloc(sizeof(float)*2*nVertex); //2 coords per vertex


				for(int j=0;j<nVertex;j++){
					textures[j*2] = mesh->mTextureCoords[0][j].x;
					textures[(j*2)+1] = mesh->mTextureCoords[0][j].y;
				}

				glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * nVertex, textures, GL_STATIC_DRAW);

			}

			//------------------------INDICES---------------------------

			vertexIndices = (unsigned int *)malloc(sizeof(unsigned int) * nFaces * 3);

		    unsigned int faceIndex = 0;

		    for(int j = 0; j<mesh->mNumFaces; j++, faceIndex += 3){
		    	aiFace* face = &mesh->mFaces[j];
		    	for(int k=0; k<face->mNumIndices; k++){
		    		memcpy(&vertexIndices[faceIndex+k], &face->mIndices[k], sizeof(unsigned int));
		    	}
		    }

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[3]);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshFaces * 3 * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * nFaces * 3, vertexIndices, GL_STATIC_DRAW);

			//we unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO
			glBindVertexArray(0);
			//unbind the rest
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
			glBindBuffer(GL_ARRAY_BUFFER,0);

			//FREE MEMORY
			free(vertex);
			free(vertexIndices);
			free(normals);
			free(textures);
		}

		success = true;
	}

	return success;
}

void AMeshResource::draw(){

	if(texture!=nullptr){
        texture->draw();
	}

	if(material){
		material->draw();
	}

	//load the buffers again
	glBindVertexArray(VAO);

	//VERTEX
	glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(GLubyte *)NULL);

	//NORMALS
	glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(GLubyte *)NULL);

	//TEXTURES
	glBindBuffer(GL_ARRAY_BUFFER,VBO[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(float),(GLubyte *)NULL);

	//INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VBO[3]);

	//Draw
	glDrawElements(GL_TRIANGLES, nFaces * 3, GL_UNSIGNED_INT, (void*)0);

	//disable buffers
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindTexture(GL_TEXTURE_2D, 0);
	

}

//--------------GETTERS------------------

GLuint* AMeshResource::getVBO() const{
	return VBO;
}
GLuint AMeshResource::getVAO() const{
	return VAO;
}
long AMeshResource::getnFaces() const{
	return nFaces;
}

ATextureResource* AMeshResource::getTextureResource() const{
	return texture;
}

AMaterialResource* AMeshResource::getMaterialResource() const{
	return material;
}

GLfloat* AMeshResource::getVertex() const{
	return vertex;
}

long AMeshResource::getnVertex() const{
	return nVertex;
}

GLuint* AMeshResource::getvertexIndices() const{
	return vertexIndices;
}


//--------------SETTERS------------------
void AMeshResource::setTextureResource(ATextureResource* t){
	texture = t;
}
void AMeshResource::setMaterialResource(AMaterialResource* m){
	material = m;
}

void AMeshResource::setMaterialID(unsigned int id){
	if(material) material->setProgramID(id);
}