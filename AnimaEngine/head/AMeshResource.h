#ifndef AMESHRESOURCE_H
#define AMESHRESOURCE_H

#include "AResource.h"
#include "ATextureResource.h"
#include "AMaterialResource.h"

class AMeshResource: public AResource
{
    public:
        AMeshResource();
        virtual ~AMeshResource();
        AMeshResource(const AMeshResource&);
        void loadUniforms(float*,float*, float*, GLuint);
        bool loadResource(std::string);
		void draw();
		//GETTERS
		GLuint* getVBO() const;
		GLuint getVAO() const;
		long getnFaces() const;
		long getnVertex() const;
		GLfloat* getVertex() const;
		GLuint* getvertexIndices() const;
		AMaterialResource* getMaterialResource() const;
		ATextureResource* getTextureResource() const;
		//SETERS
		void setTextureResource(ATextureResource*);
		void setMaterialResource(AMaterialResource*);
		void setMaterialID(unsigned int);
	private:
		long nFaces;

		// Vertex
		long nVertex;
		GLfloat* vertex;
		GLuint* vertexIndices;


		//draw buffers
		GLuint *VBO, VAO;

		//texture
		ATextureResource* texture;
		//material
		AMaterialResource* material;

};

#endif // AMESHRESOURCE_H