#include "PhysicsEngine.h"
#include "Player.h"
#include "Vector3f.h"
#include "Door.h"
#include "Dungeon.h"
#include <iostream>
/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

PhysicsEngine* PhysicsEngine::Instance(){
    static PhysicsEngine pinstance;
    return &pinstance;
}

PhysicsEngine::PhysicsEngine(){
	isDebuggin=false;
	scalar = (btScalar *)malloc(sizeof(btScalar) * 16);
	enemy_scalar = (btScalar *)malloc(sizeof(btScalar) * 16);
}

void PhysicsEngine::initPhysics(int lvl){

    m_collisionConfiguration= new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	sweepBP = new btAxisSweep3(worldMin,worldMax);
	m_overlappingPairCache = sweepBP;

	rota=0.0;

	block_p_z=false;
	block_p_x=false;

	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_constraintSolver,m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0,-9.8,0));
	m_dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;
	fileLoader = new btBulletWorldImporter(m_dynamicsWorld);

	std::string ints = std::to_string(lvl);

	std::string level = "../assets/levels/level" + ints + ".bullet";
	fileLoader->loadFile(level.c_str());

	playercallback = new btGhostPairCallback();
	worldcallback = new btGhostPairCallback();

	bulletDebugugger = new ADebug();
    m_dynamicsWorld->setDebugDrawer(bulletDebugugger);
	m3D::Motor3D::Instance()->setDebugDrawer(bulletDebugugger);
}

void PhysicsEngine::createGround(float x, float y, float z){
    //Orientation and Position
	//std::cout<<"--------1----------- Lets Ground !! ---------------------------"<<std::endl;

	btBulletWorldImporter import(0);//don't store info into the world
    if (import.loadFile("../assets/levels/mapa.bullet"))
        {
			for(int i=0; i < fileLoader->getNumRigidBodies(); i++)
			{
				btCollisionObject* obj = fileLoader->getRigidBodyByIndex(i);
				btRigidBody* body = btRigidBody::upcast(obj);		
				//btBvhTriangleMeshShape* trimeshShape = (btBvhTriangleMeshShape*)import.getCollisionShapeByIndex(i);
				//body->setUserPointer((void*)(windmillNode));
				m_dynamicsWorld->addRigidBody(body);
			}
        }
	//Objects.push_back(RigidBody);
    m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(worldcallback);
}

void PhysicsEngine::createPlayer(float x, float y, float z){

    btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin ({x, y, z});
	
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);
	sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(playercallback);
	btScalar characterHeight=0.75; //alto de la capsula
	btScalar characterWidth =19; //radio de la capsula (invertido por las coordenadas)
	capsule = new btCapsuleShape(characterWidth,characterHeight);
	m_ghostObject->setCollisionShape (capsule);
	m_ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = btScalar(0.35);
	m_character = new btKinematicCharacterController(m_ghostObject,capsule,stepHeight);
	m_dynamicsWorld->addCollisionObject(m_ghostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	m_dynamicsWorld->addAction(m_character);
	m_dynamicsWorld->setGravity(btVector3(0,-9.8,0));
	m_character->setGravity(btVector3(0,-209.8,0));

}

void PhysicsEngine::createShape(float x, float y, float z, float tx, float ty, float tz,float m){
	float TMass1= m;
	btTransform Transform1;
    Transform1.setIdentity();
    Transform1.setOrigin({x,y,z});
       
    btDefaultMotionState* MotionState1 = new btDefaultMotionState(Transform1);
       
	btVector3 shape_size= btVector3(tx,ty,tz);
    btCollisionShape* Shape1 = new btBoxShape(shape_size);
       
    btVector3 LocalInertia1;
    Shape1->calculateLocalInertia(TMass1, LocalInertia1);
       
    btRigidBody* RigidBody1 = new btRigidBody(TMass1, MotionState1, Shape1, LocalInertia1);
	RigidBody1->setRestitution(1.5);
       
    //RigidBody1->setUserPointer((void*)(box));
       
    m_dynamicsWorld->addRigidBody(RigidBody1);
}

void PhysicsEngine::createExistingShape(float x, float y, float z, float tx, float ty, float tz,float m, btRigidBody*& rb, btDefaultMotionState*& state, btCollisionShape*& bodyShape){
	btTransform Transform1;
    Transform1.setIdentity();
    Transform1.setOrigin({x,y,z});
       
    state = new btDefaultMotionState(Transform1);
       
	btVector3 shape_size= btVector3(tx,ty,tz);
    bodyShape = new btBoxShape(shape_size);
       
    btVector3 LocalInertia1;
    bodyShape->calculateLocalInertia(m, LocalInertia1);
       
    rb = new btRigidBody(m, state, bodyShape, LocalInertia1);
	//rb->setRestitution(1.5);
    //rb->setGravity(btVector3(0.0,0.0,0.0));
	//rb->setLinearVelocity(btVector3(0.0,0.0,10.0));
    m_dynamicsWorld->addRigidBody(rb);
    rb->setGravity(btVector3(0,-209.8,0));
}


void PhysicsEngine::createExistingCapsule(float x, float y, float z, btScalar Height,btScalar Width,float m, btRigidBody*& rb, btDefaultMotionState*& state, btCollisionShape*& bodyShape){
	btTransform Transform1;
    Transform1.setIdentity();
    Transform1.setOrigin({x,y,z});
       
    state = new btDefaultMotionState(Transform1);

    bodyShape = new btCapsuleShape(Width,Height);
       
    btVector3 LocalInertia1;
    bodyShape->calculateLocalInertia(m, LocalInertia1);
       
    rb = new btRigidBody(m, state, bodyShape, LocalInertia1);
	//rb->setRestitution(1.5);
    //rb->setGravity(btVector3(0.0,0.0,0.0));
	//rb->setLinearVelocity(btVector3(0.0,0.0,10.0));
    m_dynamicsWorld->addRigidBody(rb);
    rb->setGravity(btVector3(0,-209.8,0));
}

void PhysicsEngine::updatePhysics(float TDeltaTime)
{
   m_dynamicsWorld->stepSimulation(TDeltaTime, 60);

   //MODO DEBUG
   if(isDebuggin==true){
   		m_dynamicsWorld->debugDrawWorld();
		   if(bulletDebugugger!=nullptr){
			   bulletDebugugger->getLevel(Dungeon::Instance()->getLevel());
		   }
   }

   /*btIDebugDraw *prueba = m_dynamicsWorld->getDebugDrawer();
	ADebug *p = dynamic_cast<ADebug *> (prueba);
   
   std::cout<<p->GetLines().size()<<std::endl;*/
    /*vector3df Euler;
    const btQuaternion& TQuat = TObject->getOrientation();
    quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
    q.toEuler(Euler);
    Euler *= RADTODEG;
    Node->setRotation(Euler);*/
}

void PhysicsEngine::activeDebugDrawer(){

	if(isDebuggin){
		isDebuggin=false;
	}
	else{
		isDebuggin=true;
	}

}

void PhysicsEngine::activateRigidBody(bool b, btRigidBody*& btr){
	btr->activate(b);
}

void PhysicsEngine::moveRigidBody(btRigidBody*& btr, btVector3 vec){
	btr->setLinearVelocity(vec);
	//std::cout<<"MUEVO"<<std::endl;
}

void PhysicsEngine::moveEnemy(btRigidBody*& btr, float mass,float vx, float vz){
	float power = mass * 10;
	float z = 0;
	float x = 0;
	if(vx!=0){ // if player
		x+= vx * m3D::Motor3D::Instance()->getDeltaTime();
		x = x*power;

	}
	else if(vz!=0){
		z+= vz * m3D::Motor3D::Instance()->getDeltaTime();
		z = z*power;
	}
	btr->setLinearVelocity(btVector3(x,0,z));
}

void PhysicsEngine::moveEnemyLinear(btRigidBody*& btr, bool& deactivated, float vx, float vz, float& ps_x, float& ps_y, float& ps_z){
	btTransform my_trans;
    my_trans = btr->getWorldTransform();
    my_trans.getOpenGLMatrix(enemy_scalar);

    if(deactivated){
        btr->activate();
        deactivated = false;
    }

    ps_x = enemy_scalar[12];
    ps_y = ps_y;
    ps_z = enemy_scalar[14];

    btr->setLinearVelocity(btVector3(vx,0,vz));
    btr->setAngularFactor(rota);
}

void PhysicsEngine::moveDragBox(btRigidBody*& btr, float& ps_x, float& ps_y, float& ps_z){
	btTransform my_trans;
    my_trans = btr->getWorldTransform();
    my_trans.getOpenGLMatrix(scalar);

    ps_x = scalar[12];
    ps_y = ps_y;
    ps_z = scalar[14];
}

void PhysicsEngine::reposition(float x, float y, float z, btRigidBody*& btr, btDefaultMotionState*& state) {
    if(btr != nullptr){

    	btTransform transform = btr->getCenterOfMassTransform();
		transform.setOrigin(btVector3(x,y,z));
		btr->setCenterOfMassTransform(transform);
		state->setWorldTransform(transform);
	}
}

void PhysicsEngine::updatePlayerPosition(){
	auto* p = Player::Instance();
	// Seteamos el player position
	btTransform my_trans;
	my_trans = m_ghostObject->getWorldTransform();
	my_trans.getOpenGLMatrix(scalar);

	if(p->getInteractible()!=nullptr){
        if(p->getBlockMovUp()==false && p->getBlockMovDown()==false){
        	block_p_z = true;
        }
        else if(p->getBlockMovLeft()==false && p->getBlockMovRight()==false){
			block_p_x = true;
        }
    }
	else{
		block_p_x=false;
		block_p_z=false;
	}
	float ax=0;
	float az=0;

	if(block_p_x){
	 	ax = p->getX();
	}
	else{
		ax = scalar[12]; //matr.getTranslation().X;
	}

	float ay = scalar[13]; //matr.getTranslation().Y;

	if(block_p_z){
		az = p->getZ();
	}
	else{
		az = scalar[14]; //matr.getTranslation().Z;
	}
	
	Player::Instance()->setPosition(Vector3f(ax,ay,az)); //Updateamos la posicion del player
	//Player::Instance()->setRotation(matr.getRotationDegrees()); //Updateamos la rotacion del player
}

void PhysicsEngine::initMovePlayer(){
	btTransform xform;
	xform = m_ghostObject->getWorldTransform ();
	forwardDir = xform.getBasis()[2]; //para controlar el movimiento eje X
	//printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
	upDir = xform.getBasis()[1];  // para controlar el movimiento eje Z
	//btVector3 strafeDir = xform.getBasis()[0]; // para controlar el movimiento eje Y
	//printf("strafeDir=%f,%f,%f\n",strafeDir[0],strafeDir[1],strafeDir[2]);
	forwardDir.normalize ();
	upDir.normalize ();
	//strafeDir.normalize ();
	btVector3 walkDirection=btVector3(0.f,0.f,0.f);
	Player::Instance()->setWalkDirection(walkDirection);
	btScalar walkVelocity = btScalar(1.1) * Player::Instance()->getVelocity(); // 4 km/h -> 1.1 m/s
	btScalar walkSpeed = walkVelocity * m3D::Motor3D::Instance()->getDeltaTime();
	Player::Instance()->setWalkSpeed(walkSpeed);
}

void PhysicsEngine::setPlayerPosition(float x, float y, float z){
	btTransform tf = m_ghostObject->getWorldTransform();
	tf.setOrigin ({x, y, z});
	m_ghostObject->setWorldTransform(tf);
}

void PhysicsEngine::movePlayerLeft(){btVector3 vec = Player::Instance()->getWalkDirection() + forwardDir;Player::Instance()->setWalkDirection(vec);this->setMovePlayer();}
void PhysicsEngine::movePlayerRight(){btVector3 vec = Player::Instance()->getWalkDirection() - forwardDir;Player::Instance()->setWalkDirection(vec); this->setMovePlayer();}
void PhysicsEngine::movePlayerForward(){btVector3 vec = Player::Instance()->getWalkDirection() + upDir;Player::Instance()->setWalkDirection(vec); this->setMovePlayer();}
void PhysicsEngine::movePlayerBackward(){btVector3 vec = Player::Instance()->getWalkDirection()- upDir;Player::Instance()->setWalkDirection(vec); this->setMovePlayer();}

void PhysicsEngine::setMovePlayer(){
	auto* pl = Player::Instance();
	if(m_character != nullptr)
		m_character->setWalkDirection(pl->getWalkDirection()*pl->getWalkSpeed());
}


/*vector3df PhysicsEngine::getRotationDegrees(irr::core::matrix4 matr){

	vector3df vec1 = matr.getRotationDegrees();
	float x = vec1.X;
	float y = vec1.Y;
	float z = vec1.Z;

	Vector3f vec = Vector3f(x,y,z);

	return vec1;

}*/

/*vector3df PhysicsEngine::getTranslation(irr::core::matrix4 matr){

	vector3df vec1 = matr.getTranslation();
	float x = vec1.X;
	float y = vec1.Y;
	float z = vec1.Z;

	Vector3f vec = Vector3f(x,y,z);

	return vec1;

}*/

PhysicsEngine::~PhysicsEngine(){
    //dtor

	free(scalar);
	free(enemy_scalar);
}

void PhysicsEngine::clearRigidBody(btRigidBody*& body){
		btCollisionObject* obj = dynamic_cast<btCollisionObject*>(body);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete body;
}

void PhysicsEngine::clearKinematic(){

	//Limpia de forma inversa a la creacion
	if (m_character)
	{
		m_dynamicsWorld->removeCollisionObject(m_ghostObject);
		delete capsule;
		capsule = nullptr;
	}
	//Elimina los rigidsbodies del m_dynamicsworld

	//delete collision shapes
	/*for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}*/

	/*delete m_indexVertexArrays;
	delete m_vertices;*/

	if(bulletDebugugger){
		delete bulletDebugugger;
		bulletDebugugger = nullptr;
	}

	if(fileLoader){
		fileLoader->deleteAllData();
		delete fileLoader;
		fileLoader = nullptr;
	}

	if(m_ghostObject){
		delete m_ghostObject;
		m_ghostObject = nullptr;
	}


	if(m_character){
		delete m_character;
		m_character = nullptr;
	}

	if(m_constraintSolver){
		delete m_constraintSolver;
		m_constraintSolver = nullptr;
	}

	if(m_overlappingPairCache){
		delete m_overlappingPairCache;
		m_overlappingPairCache = nullptr;
	}

	if(m_dispatcher){
		delete m_dispatcher;
		m_dispatcher = nullptr;		
	}

	if(m_collisionConfiguration){
		delete m_collisionConfiguration;
		m_collisionConfiguration = nullptr;
	}

	if(playercallback){
		delete playercallback;
		playercallback = nullptr;
	}

	if(worldcallback){
		delete worldcallback;
		worldcallback = nullptr;
	}


}

