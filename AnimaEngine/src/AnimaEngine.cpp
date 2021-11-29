#include "AnimaEngine.h"
#include "ANode.h"
#include "ATransform.h"
#include "AMesh.h"
#include "AMeshLOD.h"
#include "ALight.h"
#include "ACamera.h"
#include "ABillboard.h"
#include "AAnimation.h"
#include "AResourceManager.h"
#include <iostream>

static GLuint *VBO, VAO;

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

void framebuffer_size_callback(GLFWwindow* w, int width, int height){
	glViewport(0,0,width,height);
}

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

AnimaEngine::AnimaEngine(){
	transition = false;
	skybox = nullptr;
	radius = 5;
	rm = new AResourceManager();
	debug = nullptr;
	particle_system=nullptr;
}

AnimaEngine::~AnimaEngine(){
	particle_system=nullptr;
	delete particle_system;
	debug = nullptr;
	delete debug;
	w = nullptr;
	delete scene;
	scene = nullptr;
	delete postprocess;
	postprocess = nullptr;
	delete rm;
	rm = nullptr;
	//deleting the resource manager actually deletes the actual shader because
	//it deletes the resource vectors, so we dont need to delete rs anymore.
	rs = nullptr;
	for(int i=0;i<billboards.size();i++){
		delete billboards[i];
		billboards[i] = nullptr;
	}
	for(int i=0;i<texts.size();i++){
		delete texts[i];
		texts[i] = nullptr;
	}
	if(skybox){
		delete skybox;
		skybox = nullptr;
	}
}

void AnimaEngine::init(){

	initOpenGL();
	initScene();
}

void AnimaEngine::initOpenGL(){

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	w = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"Frostyland", NULL, NULL);

	//glfwSwapInterval(1);

	if(w == NULL){
		//couldn't create window
		glfwTerminate();
		exit(0);
	}
	
	glfwMakeContextCurrent(w);
	glewInit();
	glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(5.f);
	glDepthMask(true);
	
}

void AnimaEngine::initScene(){

	//create the basic shader
	rs = rm->getShaderResource("../assets/shaders/shader");

	scene = new ANode();
	postprocess = new ANode();

	//init matrices
	ATransform* t = new ATransform();

	glm::mat4& m = t->modelMatrix();
	m = glm::mat4(1.0f);
	glm::mat4& v = t->viewMatrix();
	v = glm::mat4(1.0f);
	glm::mat4& p = t->projectionMatrix();
	p = glm::mat4(1.0f);

	delete t;

	cartoonID = glGetUniformLocation(rs->getProgramID(), "silhouette");
	//particle_system = 
}

void AnimaEngine::drawBillboard(ABillboard* b){
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setActiveShader("../assets/shaders/loading");
	rs->use();
	b->beginDraw();

	glfwSwapBuffers(w);
	glfwPollEvents();
}


void AnimaEngine::drawScene(){

	//Update particles
	updateParticles();

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//active the phong shader

	if(!transition){
		setActiveShader("../assets/shaders/shader");
		rs->use();
	}else{
		setActiveShader("../assets/shaders/transition");
		rs->use();
		GLint loc = glGetUniformLocation(rs->getProgramID(), "_radius");
		glUniform1f(loc, radius);

		GLint m_viewport[4];

		glGetIntegerv( GL_VIEWPORT, m_viewport );

		GLfloat m_vx = (m_viewport[2] * 0.25)/1080;

		glm::vec2 m_v = glm::vec2(m_vx,m_viewport[3]);

		loc = glGetUniformLocation(rs->getProgramID(), "resol");
		glUniform2fv(loc, 1, &m_v[0]);
	}

	//before drawing we need to update lights
	renderCamera();
	renderLights();
	//draw the scene

	glUniform1i(cartoonID, false);
	glEnable(GL_CULL_FACE);

	scene->draw();

	glUniform1i(cartoonID, true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCullFace(GL_FRONT);

	scene->draw();

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw Physics Debug

	if(debug!=nullptr)
		if(debug->GetLines().size()!=0)
    	debug->drawLines();

	//draw the texts (loading the specific shader)
	setActiveShader("../assets/shaders/text");
	rs->use();
	renderText();
	for(int i=0;i< texts.size();i++){
		texts[i]->beginDraw();
	}

	//draw the billboards (loading the specific shader)
	setActiveShader("../assets/shaders/billboard");
	rs->use();
	for(int i=0;i< billboards.size();i++){
		billboards[i]->beginDraw();
	}

	//RenderParticles
	if(particle_system!=nullptr){
		setActiveShader("../assets/shaders/particle");
		rs->use();
		particle_system->draw(rs->getProgramID());
	}

	//Draw skybox
	setActiveShader("../assets/shaders/skybox");
	rs->use();
	if(skybox){
    	glDepthFunc( GL_LEQUAL );  // Change depth function so depth test passes when values are equal to depth buffer's content
		skybox->beginDraw(rs->getProgramID());
    	glDepthFunc( GL_LESS ); // Set depth function back to default
	}

	drawPostProcess();

	glfwSwapBuffers(w);
	glfwPollEvents();

}

bool AnimaEngine::isActive(){
	return !glfwWindowShouldClose(w);
}

void AnimaEngine::setDebugDrawer(ADebug* p){
	debug = p;
}


void AnimaEngine::drop(){
	glfwTerminate();
}

void AnimaEngine::clearEngine(){

	debug = nullptr;

	rm->clearResourceManager();

	/*particle=nullptr;
	delete particle;*/
	debug = nullptr;
	delete debug;

	for(int i=0;i<billboards.size();i++){
		delete billboards[i];
		billboards[i] = nullptr;
	}

	billboards.resize(0);

	for(int i=0;i<texts.size();i++){
		delete texts[i];
		texts[i] = nullptr;
	}

	texts.resize(0);
}

void AnimaEngine::drawPostProcess(){

	//this method is for us to draw some meshes that need to be on top of everything else
	//we clear the depth buffer and draw the postprocess scene (tree that contais all these
	//objects)

	setActiveShader("../assets/shaders/shader");
	rs->use();

	glClear(GL_DEPTH_BUFFER_BIT);


	//CARTOON DRAW MODE (draw 2 times the scene)
	glUniform1i(cartoonID, false);
	glEnable(GL_CULL_FACE);

	postprocess->draw();

	glUniform1i(cartoonID, true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCullFace(GL_FRONT);

	postprocess->draw();

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//---------------------------
//---------CREATORS----------
//---------------------------
ANode* AnimaEngine::generateScaleRotateTraslate(glm::vec3 pos, ANode* f){

			/*scale
				|
			 rotate
			 	|
			 traslate
			 	|
			   mesh
			*/

	ANode* sn = new ANode();	
	ANode* rn = new ANode();
	ANode* tn = new ANode();

	//entities
	ATransform* s = new ATransform();
	ATransform* r = new ATransform();
	ATransform* t = new ATransform();

	sn->setEntity(s);
	rn->setEntity(r);
	tn->setEntity(t);

	//set position of the mesh
	t->traslate(pos.x,pos.y,pos.z);

	if(f){
		f->addChild(sn); 		//father is a node
	}else{
		scene->addChild(sn); 	//father is the root 
	}
	sn->addChild(rn);
	rn->addChild(tn);

	return tn;

}

ANode* AnimaEngine::generateNodesPostProcess(glm::vec3 pos){

			/*scale
				|
			 rotate
			 	|
			 traslate
			 	|
			   mesh
			*/

	ANode* sn = new ANode();	
	ANode* rn = new ANode();
	ANode* tn = new ANode();

	//entities
	ATransform* s = new ATransform();
	ATransform* r = new ATransform();
	ATransform* t = new ATransform();

	sn->setEntity(s);
	rn->setEntity(r);
	tn->setEntity(t);

	//set position of the mesh
	t->traslate(pos.x,pos.y,pos.z);

	postprocess->addChild(sn);
	sn->addChild(rn);
	rn->addChild(tn);

	return tn;

}

//---------------------MESH------------------------
ANode* AnimaEngine::createMesh(glm::vec3 p, ANode* f, std::string n,  std::string tx){

	setActiveShader("../assets/shaders/shader");

	if(!n.empty()){
		//creates a mesh
		ANode* mn = new ANode();
		AMesh* m = new AMesh();
		mn->setEntity(m);

		//set position of the mesh (creating the transform matrices)
		ANode* tn = generateScaleRotateTraslate(p, f);
		tn->addChild(mn);

		//load the resource and assign it to the mesh
		AMeshResource* res = rm->getMeshResource(n);
		m->setMeshResource(res);

		if(!tx.empty()){
			//load the texture for the mesh
			ATextureResource* rt = rm->getTextureResource(tx);
			m->setTextureResource(rt);
			
		}
		//asign the material
		AMaterialResource* rmt = rm->getMaterialResource(n);
		m->setMaterialResource(rmt);

		//sets the actual active shader to the mesh
		m->setProgramId(rs->getProgramID());

		rmt = nullptr;
		res = nullptr;
		tn = nullptr;
		m = nullptr;

		return mn;

	}else{
		std::cout << "Could not creathe the mesh" << "\n";
		exit(0);
	}
}

void AnimaEngine::createSkybox(){
	skybox = new ASkybox();
	skybox->loadSkybox();
}

ANode* AnimaEngine::createBrotherMesh(glm::vec3 p, ANode* b, std::string n, std::string tx){
	//creates a mesh brother of another node
	if(b && b != 0){
		b = b->getFather()->getFather()->getFather()->getFather();
		return createMesh(p,b,n,tx);
	}
	return 0;
}
//--------------------MESH LOD ------------------------

ANode* AnimaEngine::createMeshLOD(glm::vec3 p, ANode* f, std::string n,  std::string tx){

	setActiveShader("../assets/shaders/shader");
	if(!n.empty()){
		//creates a mesh LOD
		ANode* mn = new ANode();
		AMeshLOD* mlod = new AMeshLOD();
		mn->setEntity(mlod);
		//set position of the mesh (creating the transform matrices)
		ANode* tn = generateScaleRotateTraslate(p, f);
		tn->addChild(mn);

		AMeshLODResource* res = rm->getMeshLODResource(n);
		mlod->setMeshLODResource(res);

		if(!tx.empty()){
			for(int i = 0; i < 2; i++){
					//concatenate the file name
					std::string number = std::to_string(i);

					number = "0" + number;

					std::string png = tx + "_" + number + ".png";
					
					//load the texture
					ATextureResource* rt = rm->getTextureResource(png);
					//load it
					if(!rt){
						std::cout << "Error loading LOD texture" << "\n";
						exit(0);
					}

				//set the texture to the mesh
				mlod->setMeshLODTexture(i, rt);
			}
		}

		//asign the material
		AMaterialResource* rmt = rm->getMaterialResource(n+"_00.mtl");
		mlod->setMaterialResource(rmt);

		//sets the actual active shader to the mesh
		mlod->setProgramId(rs->getProgramID());

		rmt = nullptr;
		res = nullptr;
		tn = nullptr;
		mlod = nullptr;

		return mn;

	}else{
		std::cout << "Could not create the mesh" << "\n";
		exit(0);
	}
}

//---------------------LIGHT------------------------
ANode* AnimaEngine::createLight(glm::vec3 p, ANode* f, glm::vec3 i){

	//creates a light
	ANode* ln = new ANode();
	ALight* l = new ALight();
	ln->setEntity(l);
	l->setIntensity(i);

	//set position of the light (creating the transform matrices)
	ANode* tn = generateScaleRotateTraslate(p, f);
	tn->addChild(ln);

	//add them to the vector
	lights.push_back(ln);

	return ln;

}

ANode* AnimaEngine::createBrotherLight(glm::vec3 p, ANode* b, glm::vec3 i){
	//creates a mesh brother of another node
	if(b && b != 0){
		b = b->getFather()->getFather()->getFather()->getFather();
		return createLight(p,b,i);
	}
	return 0;
}

//------------------------ANIMATION---------------------
ANode* AnimaEngine::createAnimation(glm::vec3 p, ANode* f, std::string n, std::string t, int fr){

	setActiveShader("../assets/shaders/shader");

	if(!n.empty()){
		//creates the animation
		ANode* an = new ANode();
		AAnimation* a = new AAnimation(fr);
		an->setEntity(a);

		//set position of the mesh (creating the transform matrices)
		ANode* tn = generateScaleRotateTraslate(p, f);
		tn->addChild(an);

		//load the resource and assign it to the mesh
		AAnimationResource* res = rm->getAnimationResource(n, fr);
		a->setAnimationResource(res);

		if(!t.empty()){
			//load the texture for the mesh
			ATextureResource* rt = rm->getTextureResource(t);
			a->setTextureResource(rt);
		}

		//asign the material
		AMaterialResource* rmt = rm->getMaterialResource(n+"_000000.obj");
		a->setMaterialResource(rmt);

		//sets the actual active shader to the mesh
		a->setProgramId(rs->getProgramID());

		rmt = nullptr;
		res = nullptr;
		tn = nullptr;
		a = nullptr;

		return an;

	}else{
		std::cout << "Could not create the animation" << "\n";
		exit(0);
	}
}

ANode* AnimaEngine::createPostProcessAnimation(glm::vec3 p, std::string n, std::string t, int fr){

	setActiveShader("../assets/shaders/shader");

	if(!n.empty()){
		//creates the animation
		ANode* an = new ANode();
		AAnimation* a = new AAnimation(fr);
		an->setEntity(a);

		//set position of the mesh (creating the transform matrices)
		ANode* tn = generateNodesPostProcess(p);
		tn->addChild(an);

		//load the resource and assign it to the mesh
		AAnimationResource* res = rm->getAnimationResource(n, fr);
		a->setAnimationResource(res);

		if(!t.empty()){
			//load the texture for the mesh
			ATextureResource* rt = rm->getTextureResource(t);
			a->setTextureResource(rt);
		}

		//asign the material
		AMaterialResource* rmt = rm->getMaterialResource(n+"_000000.obj");
		a->setMaterialResource(rmt);

		//sets the actual active shader to the mesh
		a->setProgramId(rs->getProgramID());

		rmt = nullptr;
		res = nullptr;
		tn = nullptr;
		a = nullptr;

		return an;

	}else{
		std::cout << "Could not create the animation" << "\n";
		exit(0);
	}
}


//-------------------CAMERA-----------------------------
ANode* AnimaEngine::createCamera(glm::vec3 p, glm::vec3 t){
	//creates a light
	ANode* cn = new ANode();

	ACamera* c = new ACamera(30.0f,16.0/9.0,0.1f,10000.f);
	cn->setEntity(c);

	//set position of the camera (creating the transform matrices)
	ANode* tn = generateScaleRotateTraslate(glm::vec3(0.f,0.f,0.f), 0);
	tn->addChild(cn);

	//add them to the vector and set this camera as the active
	cameras.push_back(cn);
	cam = cameras.size()-1;

	//last thing to do is set the target (where is looking at) and calculate the view matrix
	setCameraParams(p,t);

	return cn;
}

//-------------------BILLBOARD-----------------------------
ABillboard* AnimaEngine::createBillboard(glm::vec3 p, std::string image, glm::vec3 scale){

	setActiveShader("../assets/shaders/billboard");
	//load the image for the billboard
	ATextureResource* res = rm->getTextureResource(image);
	//create the billboard
	ABillboard* b = new ABillboard(p, res, scale);
	
	//note that billboards are stored in a individual vector because
	//we need to procces them separatelly
	billboards.push_back(b);

	return b;
}

//-------------------PARTICLES-------------------
void AnimaEngine::createParticleSystem(std::string shape, float px, float py, float pz, float nRadius, int nBirthrate, float nParticleLife, float dx, float dy, float dz, float nBirthSize, float r, float g, float b,float a){
  AMeshResource* tm = rm->getMeshResource(shape);
	particle_system = new AParticleSystem(tm, glm::vec3(px,py,pz), nRadius, nBirthrate, nParticleLife, glm::vec3(dx,dy,dz), nBirthSize, glm::vec4(r,g,b,a));
}

//----------------------TEXT-------------------------------
AText* AnimaEngine::createText(glm::vec3 pos, std::string text, float scale, glm::vec3 color){
	//create the text
	AText* t = new AText(pos,text,scale,color);

	//note that text are stored in a individual vector because
	//we need to procces them separatelly
	texts.push_back(t);
	return t;
}

//----------------------------
//--------TRANSFORMS----------
//----------------------------
void AnimaEngine::translateNode(ANode*& n, glm::vec3 pos){
	if(n){
		ATransform* t = dynamic_cast<ATransform*> (n->getFather()->getEntity());
		glm::mat4 matrix = t->getMatrix();
		t->traslate(-matrix[3][0], -matrix[3][1], -matrix[3][2]);
		t->traslate(pos.x,pos.y,pos.z);
	}
}

void AnimaEngine::rotateNode(ANode*& n, glm::vec3 axis, float d){
	if(n){
		ATransform* r = dynamic_cast<ATransform*> (n->getFather()->getFather()->getEntity());
		r->rotate(axis.x,axis.y,axis.z,d);
	}
}

void AnimaEngine::scaleNode(ANode* n, glm::vec3 sc){
	if(n){
		ATransform* s = dynamic_cast<ATransform*> (n->getFather()->getFather()->getFather()->getEntity());
		s->scale(sc.x,sc.y,sc.z);
	}
}

glm::vec2 AnimaEngine::getScreenCoords(ANode* n){
	//https://stackoverflow.com/questions/8491247/c-opengl-convert-world-coords-to-screen2d-coords
	glm::vec2 windowSpacePos = glm::vec2(0,0);
	if(n){
		
    	ATransform* t = dynamic_cast<ATransform*> (n->getFather()->getEntity());
    	glm::mat4& m = t->modelMatrix();
    	glm::mat4& v = t->viewMatrix();
    	glm::mat4& p = t->projectionMatrix();
		glm::mat4 transla = t->getMatrix();
		glm::vec4 point3D = transla * glm::vec4(0,0,0,1);
		//We first create a 4D vector taking the position of the object we see
		glm::vec4 clipSpacePos = p * (v * point3D);
		//We take the 3D base cords and normalize to -1,1
		glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos.x / clipSpacePos.w , clipSpacePos.y / clipSpacePos.w , clipSpacePos.z / clipSpacePos.w);
		//We transform this position to a relative one in the screen
		windowSpacePos = glm::vec2( (((ndcSpacePos.x + 1.0) / 2.0) * SCR_WIDTH) , (((ndcSpacePos.y +1)/ 2.0)*SCR_HEIGHT));
	}
	return windowSpacePos;
}

//---------------------------
//---------LOADERS-----------
//---------------------------

void AnimaEngine::loadTexture(std::string image){
	rm->getTextureResource(image);
}

void AnimaEngine::loadMesh(std::string mesh){
	rm->getMeshResource(mesh);
}

void AnimaEngine::loadAnimation(std::string anim, int fr){
	rm->getAnimationResource(anim, fr);
}

//----------------------------
//----------SETTERS-----------
//----------------------------

void AnimaEngine::setWindowSize(float ww,float h){
	glfwSetWindowSize(w,ww,h);
}

void AnimaEngine::setRadius(float r){
	radius = r;
}

void AnimaEngine::setPostProcessing(bool pp){
	transition = pp;
}

void AnimaEngine::setMeshTexture(ANode*& n, std::string t){
	AMesh* m = dynamic_cast<AMesh*> (n->getEntity());
	AMeshResource* mr = m->getMeshResource();

	ATextureResource* tr = rm->getTextureResource(t);

	mr->setTextureResource(tr);

	tr = nullptr;
	m = nullptr;
	mr = nullptr;

}

void AnimaEngine::setMeshCartoon(ANode*& n, bool s){
	AMesh* m = dynamic_cast<AMesh*> (n->getEntity());	
	m->setMeshSilhouette(s);
	m = nullptr;
}

void AnimaEngine::setAnimationCartoon(ANode*& n, bool s){
	AAnimation* anim = dynamic_cast<AAnimation*> (n->getEntity());
	anim->setAnimationSilhouette(s);
	anim = nullptr;
}

void AnimaEngine::setBillboardTexture(std::string s, ABillboard* b){
	ATextureResource* tr = rm->getTextureResource(s);
	b->setTexture(tr);
	tr = nullptr;
}

void AnimaEngine::setBillboardActive(bool a, ABillboard* b){
	b->setActive(a);
}

void AnimaEngine::setBillboardPosition(ABillboard* b, glm::vec3 p){
	b->setPosition(p);
}

void AnimaEngine::setActiveShader(std::string s){
	rs = rm->getShaderResource(s);
}

void AnimaEngine::setActiveCamera(ANode* c){

}

void AnimaEngine::setCameraParams(glm::vec3 p, glm::vec3 t){

	//--------------------------------------------------------
	// To know how we can calculate the up, right and direction
	//  vectors of the camera we used the following tutorial:
	//	  https://learnopengl.com/Getting-started/Camera
	//--------------------------------------------------------

	//get the active camera
	ANode* camera = cameras[cam];
	//get its transforms
	ATransform* translation = dynamic_cast<ATransform*> (camera->getFather()->getEntity());
	ATransform* rotation = dynamic_cast<ATransform*> (camera->getFather()->getFather()->getEntity());

	glm::mat4 rotm = glm::mat4(1.0);
	glm::mat4 transm = glm::mat4(1.0);

	//calcule the right, up and direction vectors of the camera
	glm::vec3 direction = glm::normalize(t - p);
	glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0,1,0)));
	glm::vec3 up = glm::cross(right, direction);


/*	|Rx Ux Dx 0|
	|Ry Uy Dy 0|
	|Rz Uz Dz 0|
	|0 	0  0  1|
*/
	rotm[0][0] =  right.x;
	rotm[1][0] =  right.y;	
	rotm[2][0] =  right.z;	
	rotm[0][1] =  up.x;		
	rotm[1][1] =  up.y;
	rotm[2][1] =  up.z;
	rotm[0][2] =  direction.x;
	rotm[1][2] =  direction.y;
	rotm[2][2] =  direction.z;

/*	|1     0    0   0|
	|0     1    0   0|
	|0     0    1   0|
	|-Px  -Py  -Pz  1|
*/

	transm[3][0] = -p.x;
	transm[3][1] = -p.y; 
	transm[3][2] = -p.z;

	//set these transforms to the camera transforms and calcule view matrix;
	
	rotation->load(rotm);
	translation->load(transm);

	calculeViewMatrix();
}

//----------------------------
//----------GETTERS-----------
//----------------------------

GLFWwindow* AnimaEngine::getWindow(){
	return w;
}

//----------------------------
//-----------OTHERS-----------
//----------------------------
void AnimaEngine::renderText(){

	glm::mat4 p = glm::ortho(0.0,800.0,0.0,600.0);

	GLuint loc = glGetUniformLocation(rs->getProgramID(), "projectionMatrix");
	glUniform4fv(loc, 1, &p[0][0]);
}

void AnimaEngine::renderCamera(){

	glm::vec4 camerapos;

	glm::mat4 v = cameras[cam]->getFather()->getEntity()->viewMatrix();
	v = inverse(v);
	camerapos = v * glm::vec4(0,0,0,1);

	GLuint loc = glGetUniformLocation(rs->getProgramID(), "cameraPos");
	glUniform4fv(loc, 1, &camerapos[0]);
}

void AnimaEngine::renderLights(){

	GLuint loc;

	for(int i=0;i<lights.size();i++){
		//calcule light matrix
		calculeLightMatrix(lights[i]);

		//pass the variables to the shader
		//first the position
		std::string cont = std::to_string(i);
		std::string name = std::string("light[" + cont + "].position");
		glm::mat4& lm = lights[i]->getEntity()->lightMatrix();
		glm::vec4 pos =  lm * glm::vec4(0.0, 0.0, 0.0, 1.0);
		//warning: we suppose that the phong shader is activated at this point
		loc = glGetUniformLocation(rs->getProgramID(), name.c_str());
		glUniform4fv(loc, 1, &pos[0]);
		
		//now the intensity
		name = std::string("light[" + cont + "].intensity");
		loc = glGetUniformLocation(rs->getProgramID(), name.c_str());
		ALight* le = dynamic_cast<ALight*> (lights[i]->getEntity());
		glm::vec3 intens =  le->getIntensity();
		glUniform3fv(loc, 1, &intens[0]);

		//last send the light matrix

	}

	//finally we need the total number of lights
	loc = glGetUniformLocation(rs->getProgramID(), "numLights");
	glUniform1i(loc, lights.size());
}

//Renders the particles in the scene
void AnimaEngine::renderParticles() 
{
	glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(unsigned int i = 0; i < emitters.size(); i++)
	{
		AParticleSystem* e = (AParticleSystem*)emitters[i]->getEntity();
		e->draw(particlesID);
	}

	glDisable(GL_BLEND);
	
}

//Updates the particles in the scene
void AnimaEngine::updateParticles() 
{
	if(firstUpdate) 
	{
		fps = 60;
		firstUpdate = false;
	}

	for(unsigned int i = 0; i < emitters.size(); i++)
	{
		AParticleSystem* e = (AParticleSystem*)emitters[i]->getEntity();
		e->update(1/fps);
	}
	
}

void AnimaEngine::calculeLightMatrix(ANode* l){

	std::vector<ANode*> aux;

	//for this light we need to know its position in the scene so we iterate
	//trough them saving all the transformations nodes between them and the root

	ANode* nAct = l->getFather();

	while(nAct->getEntity()!=nullptr){ //if we reach the root then stop
		aux.push_back(nAct);
		nAct = nAct->getFather();
	}

	glm::mat4 lmaux = glm::mat4(1.0f);

	//now we calculate the light matrix multiplying all those matrices backwards
	for(int j=aux.size()-1;j>=0;j--){
		ATransform* tn = dynamic_cast<ATransform*> (aux[j]->getEntity());
		lmaux =  lmaux * tn->getMatrix();
	}

	//done, we have the light matrix calculated, save it
	glm::mat4& lightMatrix = l->getEntity()->lightMatrix();
	lightMatrix = lmaux;
}

void AnimaEngine::calculeViewMatrix(){

	std::vector<ANode*> aux;

	//for the active camera we need to get all its transform until we reach the root
	ANode* nAct = cameras[cam]->getFather();

	while(nAct->getEntity()!=nullptr){ //if we reach the root then stop
		aux.push_back(nAct);
		nAct = nAct->getFather();
	}
	
	glm::mat4 vmaux = glm::mat4(1.0f);

	//now we calculate the lights matrix multiplying all those matrices but
	//backwards
	for(int j=aux.size()-1;j>=0;j--){
		ATransform* tn = dynamic_cast<ATransform*> (aux[j]->getEntity());
		vmaux = tn->getMatrix() * vmaux ;
	}

	//done, we have the light matrix calculated, save it
	glm::mat4& viewMatrix = cameras[cam]->getEntity()->viewMatrix();
	viewMatrix = vmaux;
}

//----------------------------
//-----------DTOR-------------
//----------------------------

void AnimaEngine::eraseTextEntity(AText*& t){
	for(int i=0; i<texts.size(); i++){
		if(t == texts[i]){
			delete texts[i];
			texts[i] = nullptr;
			texts.erase(texts.begin()+i);
			break;
		}
	}
}

void AnimaEngine::eraseNode(ANode*& n){
	//erases a node of the scene tree and its transforms

	//first, locate the first transform of this node (the scale)
	/*
							  scale  <--
								|
							 rotate
							 	|
							 traslate
							 	|
							    n
	*/
	ANode* scale = n->getFather()->getFather()->getFather();

	//then we save a pointer to its father because we need to erase the scale node from the father's child vector
	ANode* father = scale->getFather();

	//this method will delete the scale node (causing that all its childs got deleted in cascade) and will remove it
	//from father's child vector
	father->removeChild(scale);

	scale = nullptr;
	father = nullptr;
}

void AnimaEngine::eraseMeshResource(AMeshResource* me){
	//erases a mesh loaded
	rm->eraseMeshResource(me);
}

void AnimaEngine::eraseTextureResource(ATextureResource* t){
	//erases a texture loaded
	rm->eraseTextureResource(t);
}

void AnimaEngine::eraseMaterialResource(AMaterialResource* ma){
	//erases a material loaded
	rm->eraseMaterialResource(ma);
}

void AnimaEngine::eraseMeshNode(ANode*& n){
	//calls erase texture, erase mesh and erase node to erase a mesh node completely from the scene

	AMesh* meshEntity = dynamic_cast<AMesh*> (n->getEntity());

	eraseTextureResource(meshEntity->getTextureResource());

	eraseMaterialResource(meshEntity->getMaterialResource());

	eraseMeshResource(meshEntity->getMeshResource());

	eraseNode(n);
}


void AnimaEngine::eraseSkybox(){
	if(skybox){
		delete skybox;
		skybox = nullptr;
	}
}

void AnimaEngine::eraseParticleSystem(){
	if(particle_system){
		delete particle_system;
		particle_system=nullptr;
	}
}

//----------------------------
//-----ANIMATION HANDLERS-----
//----------------------------
void AnimaEngine::stopAnimation(ANode*& n){

	AAnimation* anim = dynamic_cast<AAnimation*> (n->getEntity());

	anim->stop();
}

void AnimaEngine::playAnimation(ANode*& n){

	AAnimation* anim = dynamic_cast<AAnimation*> (n->getEntity());

	anim->play();
}

void AnimaEngine::resetAnimation(ANode*& n){

	AAnimation* anim = dynamic_cast<AAnimation*> (n->getEntity());

	anim->reset();
}

void AnimaEngine::setLoopAnimation(ANode*& n, bool l){

	AAnimation* anim = dynamic_cast<AAnimation*> (n->getEntity());

	anim->setLoop(l);
}

void AnimaEngine::setActFrame(ANode*& n, int f){
	AAnimation* anim = dynamic_cast<AAnimation*> (n->getEntity());

	anim->setFrame(f);
}

void AnimaEngine::setAnimationActive(ANode*& a, bool f){
	AAnimation* anim = dynamic_cast<AAnimation*> (a->getEntity());
	
	anim->setAnimationActive(f);
}

void AnimaEngine::changeAnimation(ANode*& a, std::string n, std::string t, int fr){
	setActiveShader("../assets/shaders/shader");

	if(!n.empty()){
		AAnimation* anim = dynamic_cast<AAnimation*> (a->getEntity());

		anim->stop();
		anim->reset();
		anim->setLoop(false);
		anim->setFrames(fr);

		//load the resource and assign it to the mesh
		AAnimationResource* res = rm->getAnimationResource(n, fr);
		anim->setAnimationResource(res);

		if(!t.empty()){
			//load the texture for the mesh
			ATextureResource* rt = rm->getTextureResource(t);
			anim->setTextureResource(rt);
		}

		//asign the material
		AMaterialResource* rmt = rm->getMaterialResource(n+"_000000.obj");
		anim->setMaterialResource(rmt);

		//sets the actual active shader to the mesh
		anim->setProgramId(rs->getProgramID());

		rmt = nullptr;
		res = nullptr;
		anim = nullptr;

	}else{
		std::cout << "Could not create the animation" << "\n";
		exit(0);
	}
}

void AnimaEngine::setText(AText*& t, std::string text){
	t->setText(text);
}

void AnimaEngine::setTextPosition(AText* t, glm::vec3 p){
	t->setPosition(p);
}

void AnimaEngine::setActiveLOD(ANode*& n, bool sn){
		
		AMeshLOD* meshlod = dynamic_cast<AMeshLOD*> (n->getEntity());
		if(sn == false){
			meshlod->setActive(0);
		}else{
			meshlod->setActive(1);
		}
}