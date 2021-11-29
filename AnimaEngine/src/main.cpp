#include <glew.h>
#include <gl.h>
#include <glfw3.h>
#include "AnimaEngine.h"
#include "ANode.h"
#include "AEntity.h"
#include "AMesh.h"
#include "ATransform.h"
#include "ABillboard.h"
#include "AResourceManager.h"
#include "AMeshResource.h"
#include "AShaderResource.h"
#include "ATextureResource.h"
#include "ADebug.h"
#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>


#define UNUSED_SHADER_ATTR -1

static btDiscreteDynamicsWorld* dynamicsWorld;

void initPhysics()
{
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    btCollisionShape* groundShape = new btBoxShape(btVector3(5, 5, 5));
    btCollisionShape* fallShape = new btBoxShape(btVector3(1, 1, 1));


    // Orientation and Position of Ground
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -3, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);

    // Orientation and Position of Falling body
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-1, 10, 0)));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);

    ADebug *bulletDebugugger = new ADebug();
    //bulletDebugugger.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    dynamicsWorld->setDebugDrawer(bulletDebugugger);
	//eng->setDebugDrawer(bulletDebugugger);
    //dynamicsWorld->getDebugDrawer()->setDebugMode(1);

}

int main(){

	AnimaEngine* eng = new AnimaEngine();
	eng->init();
	initPhysics();

	//create a mesh
	ANode* me = eng->createMesh(glm::vec3(0,0,0), 0,"../assets/caja.obj", "../assets/caja.jpg");
	//eng->scaleNode(me, glm::vec3(4,4,4));
	
	//create a point light
	ANode* l = eng->createLight(glm::vec3(5,1,-5), 0, glm::vec3(0.4,0.4,0.4));

	//create the scene camera
	ANode* cam = eng->createCamera(glm::vec3(2,30,50),glm::vec3(0,0,0));

	//ABillboard* b = eng->createBillboard(glm::vec3(-1,-1,0), "../assets/heart.png", glm::vec3(0.004, 0.004, 1));
    //ABillboard* ba = eng->createBillboard(glm::vec3(0,0.5f,-0.5f), "../assets/heart.png", glm::vec3(0.004, 0.004, 1));
    eng->createParticleSystem("../assets/caja.obj",0,-100,0,100,250,5,0,0.5f,0,0.3f,1,1,1,1);

	//AText* t = eng->createText(glm::vec3(-1.0,-0.5,0.0), "prueba", 0.0005, glm::vec3(1.0,0.0,0.0));

	//animation
	//ANode* anim = eng->createAnimation(glm::vec3(0,0,0), 0, "../assets/animations/cofresillo/cofre-riggin", "../assets/cofre.png", 51);
	//eng->playAnimation(anim);
	//eng->setLoopAnimation(anim,true);

	
	//PRUEBAS DEBUG
	btIDebugDraw *prueba = dynamicsWorld->getDebugDrawer();
	ADebug *p = dynamic_cast<ADebug *> (prueba);
	eng->setDebugDrawer(p);

	while(eng->isActive()){
		eng->drawScene();
        
		//Fisicas
        dynamicsWorld->debugDrawWorld();
		dynamicsWorld->stepSimulation(0.01f, 60);
        //std::cout<<dynamicsWorld->getDebugDrawer()->getDebugMode()<<std::endl;
        //std::cout<<p->GetLines().size()<<std::endl;
        //std::cout<<dynamicsWorld->getDebugDrawer()<<std::endl;
	}

	eng->drop();
	delete eng;

	return 0;
}
