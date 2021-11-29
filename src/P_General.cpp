#include "P_General.h"
#include "Player.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include <iostream>
#include "Room.h"
#include "TaskTypes.h"
#include "AudioManager.h"

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

P_General::P_General(Vector3f pos){

    //Characteristics
	vida = 10;
    maxlife = 10;
    ataque = 3;
    velocidadX = 1.f;
    velocidadZ = 0.f;
    ps_x = pos.getX();
    ps_y = pos.getY();
    ps_z = pos.getZ();
    maxvel = 25.f;
    coins = 10;
    width = 30;
    angle = 0;
    rleft = true;
    h_r = 80;
    s_r = 190;
    height = 40;
    chaseando = false;
    salaActual = -1;
    death = false;
    state = 0;

    follow = false;
    followup = -1;
    setMass(10.0f);
    
    //Where is looking at
    vision_vector = new Vector3f(1.f,0.f,0.f);

    //Path to follow
    path = new Path(-192, 38, 74, 276, true);

    //Generate Behavior Tree for Penguin Soldier Enemie Type
    generateBehaviorTree();

    memory = new SMemory();

    //Physics
    PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,10.5f,10.5f,10.0f,10.f,rigidBody,phy_state,bodyShape);

    //--------Animation------------
    animationtime = 0.0;
    anim_state = 0;
    animationframe = 1;
    animation = false;
    a_time = -1;
}

P_General::~P_General(){
    if(!death){
        m3D::Motor3D::Instance()->clearNode(soldier);
        m3D::Motor3D::Instance()->clearNode(shadow);
        if(rigidBody){
            delete rigidBody;
            rigidBody = nullptr;
        }
        if(phy_state){
            delete phy_state;
            phy_state = nullptr;
        }
        if(bodyShape){
            delete bodyShape;
            bodyShape = nullptr;
        }
    }
    delete memory;
    memory = nullptr;
    delete path;
    path = nullptr;
    delete vision_vector;
    vision_vector = nullptr;
    delete selector_task;
    selector_task = nullptr;
    delete follow_task;
    follow_task = nullptr;
}

/////////////////////////////////////////
// MAIN METHODS  
/////////////////////////////////////////


void  P_General::updateEntity(){

    if(!death){
 
        auto* m = m3D::Motor3D::Instance();
        auto* d = Dungeon::Instance();
        auto* f = PhysicsEngine::Instance();

        //Update State/Action Memory
        updatePEntities();

        //Animation
        if(velocidadX != 0 || velocidadZ != 0){
            updateAnimation();
        }else{
            m->setMeshTexture(soldier,"../assets/textures/pinguG1.png");
        }

        //Run Behavior Tree
        if(!follow && selector_task){
            selector_task->execute(getID());
        }else if(follow_task){
            follow_task->execute(getID());
        }

        if(rigidBody!=nullptr){
            f->moveEnemyLinear(rigidBody, deactivated, velocidadX, velocidadZ, ps_x, ps_y, ps_z);
        }

        if(velocidadX < 0){
            rleft = true;
        }else{
            rleft = false;
        }

        if(rleft && angle != 0){
            angle -= 20;
            m->rotateEntity(soldier, -20);
        }else if(!rleft && angle != 180){
            angle += 20;
            m->rotateEntity(soldier, 20);
        }

        m->setPositionEntity(soldier,ps_x,ps_y,ps_z);
        m->setPositionEntity(shadow,ps_x,ps_y,ps_z);

        int aux = d->wichRoomIAmAt(ps_x, ps_z);

        if(salaActual != aux && aux != -1){
            if(salaActual != -1){
                Room* room = d->getRoomPerID(salaActual);
                room->quitAlert(1);
                room = nullptr;
            }
            salaActual = aux;
            recalculatePath();
        }
        Room* room2 = d->getRoomPerID(salaActual);
        if(room2->getType()==HOLLOW_ROOM && ps_x < -186.0f && ps_z > 48.0f){
            rigidBody->setLinearVelocity(btVector3(0,-49.8,0));
            if(ps_y<-135.0f){
                Die();
            }
        }
    }
}

void P_General::updateAnimation(){

    auto* m = Motor3D::Instance();

    if(animationtime > 0.2){

        if(animationframe == 1 || animationframe == 3){
            m->setMeshTexture(soldier,"../assets/textures/pinguG1.png");
        }else if(animationframe == 2){
            m->setMeshTexture(soldier,"../assets/textures/pinguG2.png");
        }else if(animationframe == 4){
            m->setMeshTexture(soldier,"../assets/textures/pinguG3.png");
        }
        
        animationframe++;

        if(animationframe == 5)
            animationframe = 1;

        animationtime = 0.f;
    }else{
        animationtime += m->getDeltaTime();
    }
}

void P_General::recalculatePath(){
        
    auto* d = Dungeon::Instance();
    Room* room = d->getRoomPerID(salaActual);
    
    if(room != NULL){
        room->addAlert(1);
        path->recalculatePath(room->getCenterX(), room->getCenterZ(), room->getWidth(), room->getHeight());
    }

    d = nullptr;
    room = nullptr;
	
}

void P_General::generateBehaviorTree(){

/*                               <Selector>
//                                   |?
//                  _____________________________________
//                 /                 |                   \
//                /                  |                    \
//           <Sequence>          <Sequence>           <PatrolTask>
//               |->                 |->
//               |             ______________ 
//               |            /              \                          
//               |           /                \                           
//               |      <CheckPlayer>       <MoveTo>         
//               |             
//         __________________________________
//        /                |                 \                 
//       /                 |                  \                   
//  <DetectTask>       <ChaseTask>         <AttackTask>  
*/       

    selector_task = new Selector();
    //First branch
    Sequence* sequence = new Sequence();
    DetectTask* detect = new DetectTask();
    ChaseTask* chase = new ChaseTask();
    AttackTask* attack = new AttackTask();
    sequence->addTask(detect);
    sequence->addTask(chase);
    sequence->addTask(attack);
    //Second branch
    Sequence* sequence2 = new Sequence();
    MoveTo* mvto = new MoveTo();
    CheckPlayer* cp = new CheckPlayer();
    sequence2->addTask(cp);
    sequence2->addTask(mvto);
    //Last leaf
    PatrolTask* ptask = new PatrolTask();
    //Mix it
    selector_task->addTask(sequence);
    selector_task->addTask(sequence2);
    selector_task->addTask(ptask);

    follow_task = new Selector();
    FollowTask* ft = new FollowTask();
    Sequence* sequence3 = new Sequence();
    ResetBehavior* reset = new ResetBehavior();
    DetectTask* detect2 = new DetectTask();
    sequence3->addTask(detect2);
    sequence3->addTask(reset);
    follow_task->addTask(sequence3);
    follow_task->addTask(ft);

    mvto = nullptr;
    cp = nullptr;
    reset = nullptr;
    sequence3 = nullptr;
    sequence2 = nullptr;
    sequence = nullptr;
    detect = nullptr;
    chase = nullptr;
    attack = nullptr;
    detect = nullptr;
}

void P_General::playAudio(int a){
    auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.3){
        r = 0.4;
    }else if(r == 0.5){
        r = 0.4;
    }else if(r == 0.6){
        r = 0.55;
    }
    
    if(a==0){
        //Attack
        am->shoot3DAudio("Voces/AI/Pgeneral/Atacar",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==1){
        //RDamage
        am->shoot3DAudio("Voces/AI/Pgeneral/RDaño",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==2){
        //Die
        am->shoot3DAudio("Voces/AI/Pgeneral/Morir",0,r, Vector3f(ps_x, ps_y, ps_z));

    }

    am = nullptr;
}