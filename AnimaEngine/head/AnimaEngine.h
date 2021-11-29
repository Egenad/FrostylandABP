#ifndef ANIMAENGINE_H
#define ANIMAENGINE_H

#include <vector>
#include "AResourceManager.h"
#include "ANode.h"
#include "ABillboard.h"
#include "AText.h"
#include "ADebug.h"
#include "ASkybox.h"
#include "AParticleSystem.h"

struct ViewPort{
	float _x;
	float _y;
	int width;
	int height;
};

class AnimaEngine
{
    public:
        AnimaEngine();
        virtual ~AnimaEngine();

		void init();   			              //Anima Engine init
		void drawBillboard(ABillboard*);
		void drawScene();		              //draws the current scene
		void drop();   			              //drops Anima Engine ->MEJORARLO
		void clearEngine();
		void drawPostProcess();

		void initOpenGL();
		void initScene();

		//CREATORS
		ANode* createMesh(glm::vec3, ANode*, std::string, std::string);				//creates a mesh and set it in the position
		void createSkybox();
		ANode* createBrotherMesh(glm::vec3, ANode*, std::string, std::string);		//creates a mesh brother of another node
		ANode* createMeshLOD(glm::vec3, ANode*, std::string, std::string);				//creates an array of meshes and set it in the position
		ANode* createLight(glm::vec3, ANode*, glm::vec3);							//creates a light
		ANode* createBrotherLight(glm::vec3, ANode*, glm::vec3);					//creates a light brother of another node
		ANode* createAnimation(glm::vec3, ANode*, std::string, std::string, int);	//creates an animation
		ABillboard* createBillboard(glm::vec3, std::string, glm::vec3);   			//creates a billboard 
		void createParticleSystem(std::string shape, float px, float py, float pz, float nRadius, int nBirthrate, float nParticleLife, float dx, float dy, float dz, float nBirthSize, float r, float g, float b, float a);
    	//void createParticleSystem(const char* shape, glm::vec3 nPosition, float nRadius, int nBirthrate, float nParticleLife, glm::vec3 nBirthDirection, glm::vec3 nDeathDirection, float nVariationDirection, float nBirthSize, float nDeathSize, float nVariationSize, glm::vec4 nBirthColor, glm::vec4 nDeathColor, float nVariationColor);
		AText* createText(glm::vec3, std::string, float, glm::vec3);				//creates a text
		ANode* createCamera(glm::vec3, glm::vec3); 									//parameters: position of the camera and where it looks at				 
		//void createViewPort(float, float, int, int);
		ANode* createPostProcessAnimation(glm::vec3, std::string, std::string, int);
		
		//LOD
		void setActiveLOD(ANode*&, bool);

		//LOADERS
		void loadTexture(std::string);
		void loadMesh(std::string);
		void loadAnimation(std::string, int);

		//TRANSFORM OPERATIONS
		void translateNode(ANode*&, glm::vec3);
		void rotateNode(ANode*&, glm::vec3, float);
		void scaleNode(ANode*, glm::vec3);
		glm::vec2 getScreenCoords(ANode*);

		//ANIMATION HANDLERS
		void stopAnimation(ANode*&);
		void playAnimation(ANode*&);
        void resetAnimation(ANode*&);
        void setLoopAnimation(ANode*&, bool);
        void setActFrame(ANode*&, int);
        void changeAnimation(ANode*&, std::string, std::string, int);

		bool isActive();

		//SETTERS
		void setActiveShader(std::string);			//loads the shader and sets it active
		void setActiveCamera(ANode*);
		void setCameraParams(glm::vec3, glm::vec3);
		void setDebugDrawer(ADebug*);
		//void setLightIntensity(ANode*);
		void setMeshTexture(ANode*&, std::string);
		void setMeshCartoon(ANode*&, bool);
		void setAnimationCartoon(ANode*&, bool);
		void setBillboardTexture(std::string, ABillboard*);
		void setBillboardActive(bool, ABillboard*);
		void setBillboardPosition(ABillboard*, glm::vec3);
		void setAnimationActive(ANode*&, bool);
		void setText(AText*&, std::string);
		void setTextPosition(AText*, glm::vec3);
		void setPostProcessing(bool);
		void setRadius(float);
		void setWindowSize(float,float);

		//GETTERS
		GLFWwindow* getWindow();

		//DTOR
		void eraseNode(ANode*&);		 				//erases a node of the scene tree and its transforms
		void eraseMeshResource(AMeshResource*);			//erases a mesh loaded
		void eraseTextureResource(ATextureResource*);	//erases a texture loaded
		void eraseMaterialResource(AMaterialResource*); //erases a material loaded
		void eraseMeshNode(ANode*&);   
		void eraseTextEntity(AText*&);					//calls erase texture, erase mesh and erase node to erase a mesh node completely from the scene
		void eraseSkybox();								//erases the skybox
		void eraseParticleSystem();						//erases the particle_system

    private:

		ANode* generateScaleRotateTraslate(glm::vec3, ANode*);
		ANode* generateNodesPostProcess(glm::vec3);
		void calculeLightMatrix(ANode*);
		void calculeViewMatrix();
		void renderLights();
		void renderCamera();
		void renderText();
		void renderParticles();
		void updateParticles();

    	AResourceManager* rm;
    	ANode* scene, *postprocess;
    	ASkybox* skybox;

    	//active shader
    	AShaderResource* rs;
    	unsigned int cartoonID;
    	//active camera
    	int cam;
    	bool transition;
    	float radius;

    	std::vector<ANode*> lights;
    	std::vector<ANode*> cameras;
    	std::vector<ABillboard*> billboards;
    	std::vector<AText*> texts;
		ADebug* debug;

		AParticleSystem* particle_system;

    	//window
    	GLFWwindow* w;
    	unsigned int postBuffer;
    	unsigned int renderedTexture;
    	unsigned int depthTexture;
    	float DrawBuffers[1];

    	const float g_quad_vertex_buffer_data[18] = { 
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
		};

		// Particles
		std::vector<ANode*> emitters;
    	GLuint particlesID;
		GLuint paticlesVertexArray;
		bool firstUpdate = true;
		double fps = 0;
		AParticleSystem* partycle_system;
};

#endif // ANIMAENGINE_H