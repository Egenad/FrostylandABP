#include "P_Soldier.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include "Room.h"
#include "TaskTypes.h"
#include "Player.h"
#include "AudioManager.h"

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

P_Soldier::P_Soldier(Vector3f pos){

    //Characteristics
	vida = 6;
    maxlife = 6;
    ataque = 1;
    velocidadX = 0.f;
    velocidadZ = 0.f;
    width = 30;
    height = 40;
    ps_x = pos.getX();
    ps_y = pos.getY();
    ps_z = pos.getZ();
    h_r = 70;
    s_r = 180;
    angle = 0;
    rleft = true;
    maxvel = 50.f;
    coins = 5;
    chaseando = false;
    salaActual = -1;
    death = false;
    state = 0;
    setMass(10.0f);
    deactivated = true;

    follow = false;
    followup = -1;
    
    //Where is looking at
    vision_vector = new Vector3f(1.f,0.f,0.f);

    //Path to follow
    path = new Path(-192, 38, 74, 276, true);

    //Generate Behavior Tree for Penguin Soldier Enemie Type
    selector_task = nullptr;
    follow_task = nullptr;
    generateBehaviorTree();

    memory = new SMemory();

    //Physics
    PhysicsEngine::Instance()->createExistingCapsule(ps_x,ps_y,ps_z,0.17,17,10.f,rigidBody,phy_state,bodyShape);

    //--------Animation------------
    anim_state = 0;
    animationtime = 0.0;
    animationframe = 1;
    animation = false;
    a_time = -1;
}

P_Soldier::~P_Soldier()
{
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

void P_Soldier::updateEntity(){

    if(!death){

        auto* m = m3D::Motor3D::Instance();
        auto* d = Dungeon::Instance();
        auto* p = Player::Instance();
        auto* f = PhysicsEngine::Instance();

        int aux = d->wichRoomIAmAt(ps_x, ps_z);

        if(salaActual != aux && aux != -1){
            if(salaActual != -1){
                Room* room = d->getRoomPerID(salaActual);
                room->quitAlert(1);
                room = nullptr;
            }
            salaActual = aux;
            recalculatePath();
        }if(salaActual == -1){
            Die();
        }

        //Update State/Action Memory
        updatePEntities();

        //Animation
        if(velocidadX != 0 || velocidadZ != 0){
            updateAnimation();
        }else{
            m->setMeshTexture(soldier,"../assets/textures/pingu.png");
        }

        //Run Behavior Tree
        if(!follow && selector_task){
            selector_task->execute(getID());
        }else if(follow_task){
            if(followup != -1){
                follow_task->execute(getID());
            }else{
                follow = false;
            }
        }

        if(rigidBody!=nullptr){
            deactivated = true;
            f->moveEnemyLinear(rigidBody, deactivated, velocidadX, velocidadZ, ps_x, ps_y, ps_z);
        }

        if(vision_vector->getX() < 0){
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

        Room* room2 = d->getRoomPerID(salaActual);
        if(room2->getType()==HOLLOW_ROOM && ps_x < -186.0f && ps_z > 48.0f){
            rigidBody->setLinearVelocity(btVector3(0,-49.8,0));
            if(ps_y<-135.0f){
                Die();
            }
        }

        //This code is for when the player attacks the enemy.

        if(animation){
            a_time=m->getNow()+0.5;
            animation=false;
            setPushVector(p->getVisionVectorX(), -1, getVisionVectorZ());
        }
        if(a_time!=-1){
            if(a_time>m->getNow()){
                if(rigidBody!=nullptr)
                    f->moveEnemy(rigidBody,getMass(),80*push_vector.getX(),80*(-push_vector.getZ()));
            }
            else{
                animation=false;
                a_time=-1;
                setPushVector(0,0,0);
            }
        }

        p = nullptr;
        m = nullptr;
        d = nullptr;
        f = nullptr;
    }
}

void P_Soldier::updateAnimation(){

    auto* m = Motor3D::Instance();

    if(animationtime > 0.2){

        animationtime = 0.f;

        if(animationframe == 1 || animationframe == 3){
            m->setMeshTexture(soldier,"../assets/textures/pingu.png");
        }else if(animationframe == 2){
            m->setMeshTexture(soldier,"../assets/textures/pingu2.png");
        }else if(animationframe == 4){
            m->setMeshTexture(soldier,"../assets/textures/pingu3.png");
        }
        
        animationframe++;

        if(animationframe == 5)
            animationframe = 1;
        
    }else{
        animationtime += m->getDeltaTime();
    }
}

void P_Soldier::generateBehaviorTree(){

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

    sequence = nullptr;
    detect = nullptr;
    chase = nullptr;
    attack = nullptr;
    sequence2 = nullptr;
    ptask = nullptr;
    mvto = nullptr;
    cp = nullptr;

    follow_task = new Selector();
    FollowTask* ft = new FollowTask();
    Sequence* sequence3 = new Sequence();
    ResetBehavior* reset = new ResetBehavior();
    DetectTask* detect2 = new DetectTask();
    sequence3->addTask(detect2);
    sequence3->addTask(reset);
    follow_task->addTask(sequence3);
    follow_task->addTask(ft);

    reset = nullptr;
    sequence3 = nullptr;
    ft = nullptr;
    detect2 = nullptr;
}

void P_Soldier::recalculatePath(){
    auto* d = Dungeon::Instance();
    Room* room = d->getRoomPerID(salaActual);
    
    if(room != NULL){
        room->addAlert(1);
        //std::cout << salaActual << std::endl;
        path->recalculatePath(room->getCenterX(), room->getCenterZ(), room->getWidth(), room->getHeight());
    }

    d = nullptr;
    room = nullptr;
}

void P_Soldier::playAudio(int a){
    auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.3){
        r = 0.4;
    }else if(r == 0.5){
        r = 0.4;
    }else if(r == 0.6){
        r = 0.55;
    }
    
    //std::cout<< "random Soldier -> " << r << "\n";

    if(a==0){
        //Attack
        am->shoot3DAudio("Voces/AI/Psoldado/Atacar",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==1){
        //RDamage
        am->shoot3DAudio("Voces/AI/Psoldado/RDaño",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==2){
        //Die
        am->shoot3DAudio("Voces/AI/Psoldado/Morir",0,r, Vector3f(ps_x, ps_y, ps_z));

    }

    am = nullptr;
}

void P_Soldier::setFollow(bool f){
    follow = f;
}

bool P_Soldier::getFollow(){
    return follow;
}

int P_Soldier::getFollowUp(){
    return followup;
}

void P_Soldier::setFollowUp(int s){
    followup = s;
}
