#include "Fox.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include "Room.h"
#include "TaskTypes.h"
#include "AudioManager.h"

Fox::Fox(Vector3f pos){
	//Characteristics
	vida = 6;
    maxlife = 6;
    ataque = 1;
    velocidadX = 1.f;
    velocidadZ = 0.f;
    width = 30;
    height = 40;
    ps_x = pos.getX();
    ps_y = pos.getY();
    ps_z = pos.getZ();
    ini_x = pos.getX();;
    ini_y = pos.getY();;
    ini_z = pos.getZ();;
    maxvel = 50.f;
    coins = 5;
    h_r = 110;
    s_r = 210;
    angle = 0;
    rleft = true;
    chaseando = false;
    salaActual = -1;
    death = false;
    state = 0;
    setMass(10.0f);
    deactivated = false;
    //Where is looking at
    vision_vector = new Vector3f(1.f,0.f,0.f);

    //Path to follow
    path = new Path(-192, 38, 74, 276, true);

    //Generate Behavior Tree for Penguin Soldier Enemie Type
    selector_task = nullptr;
    generateBehaviorTree();

    memory = new SMemory();
    pathlist = nullptr;

    //Physics
    PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,10.5f,10.5f,10.0f,10.f,rigidBody,phy_state,bodyShape);

    //--------Animation------------
    anim_state = 0;
    animationtime = 0.0;
    animationframe = 1;
    animation = false;
    a_time = -1;
}

Fox::~Fox(){
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
    if(pathlist){
        delete pathlist;
        pathlist = nullptr;
    }
    delete memory;
    memory = nullptr;
    delete path;
    path = nullptr;
    delete vision_vector;
    vision_vector = nullptr;
    delete selector_task;
    selector_task = nullptr;
}

void Fox::updateEntity(){

    if(!death){
    	auto* d = Dungeon::Instance();
        auto* m = m3D::Motor3D::Instance();

    	//Update State/Action Memory
        updatePEntities();

        //Run Behavior Tree
        if(selector_task)
            selector_task->execute(getID());

        lastpos.setX(ps_x);
        lastpos.setY(ps_y);
        lastpos.setZ(ps_z);

        ps_x += velocidadX * m->getDeltaTime();
        ps_z += velocidadZ * m->getDeltaTime();

        if(rigidBody!=nullptr){
            deactivated = true;
            PhysicsEngine::Instance()->moveEnemyLinear(rigidBody, deactivated, velocidadX, velocidadZ, ps_x, ps_y, ps_z);
        }

        if(velocidadX != 0 || velocidadZ != 0){
            updateAnimation();
        }else{
            m->setMeshTexture(soldier,"../assets/textures/zorro1.png");
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
        }else if(salaActual == -1){
            Die();
        }
    }
}

void Fox::updateAnimation(){

    auto* m = Motor3D::Instance();

    if(animationtime > 0.2){

        if(animationframe == 1 || animationframe == 3){
            m->setMeshTexture(soldier,"../assets/textures/zorro1.png");
        }else if(animationframe == 2){
            m->setMeshTexture(soldier,"../assets/textures/zorro2.png");
        }else if(animationframe == 4){
            m->setMeshTexture(soldier,"../assets/textures/zorro3.png");
        }
        
        animationframe++;

        if(animationframe == 5)
            animationframe = 1;

        animationtime = 0.f;
    }else{
        animationtime += m->getDeltaTime();
    }
}

void Fox::recalculatePath(){
	auto* d = Dungeon::Instance();
    Room* room = d->getRoomPerID(salaActual);
    
    if(room != NULL){
        room->addAlert(1);
        path->recalculatePath(room->getCenterX(), room->getCenterZ(), room->getWidth(), room->getHeight());
    }

    d = nullptr;
    room = nullptr;
}

void Fox::generateBehaviorTree(){
/*                              <Selector>
//                                  |?
//             __________________________________________________
//            /                |                \                \
//           /                 |                 \                \
//      <Sequence>        <MoveToNode>        <Sequence>       <PatrolTask>
//           |->                                   |->
//       __________________                   ________________
//      /                  \                  \               \
//     /                    \                  \               \
// <CheckPlayer>    <CalculateShortPath>   <DetectTask>      <AvoidTask>
//                            */

	selector_task = new Selector();
    //First branch
    Sequence* sequence = new Sequence();

    CheckPlayer* ctask = new CheckPlayer();
    CalculateShortPath* calc = new CalculateShortPath();

    MoveToNode* mvt = new MoveToNode();

    PatrolTask* ptask = new PatrolTask();
    sequence->addTask(ctask);
    sequence->addTask(calc);

    Sequence* sequence2 = new Sequence();
    DetectTask* detect = new DetectTask();
    AvoidTask* avoid = new AvoidTask();

    sequence2->addTask(detect);
    sequence2->addTask(avoid);
    //Mix it
    selector_task->addTask(sequence);
    selector_task->addTask(mvt);
    selector_task->addTask(sequence2);
    selector_task->addTask(ptask);

    ptask = nullptr;

}

PathfindingList* Fox::getPathList(){
    return pathlist;
}

void Fox::setPathList(PathfindingList list){
    if(pathlist != nullptr){
        delete pathlist;
        pathlist = nullptr;
    }
    pathlist = new PathfindingList(list);
    //pathlist->draw();
}

void Fox::removePathList(){
    if(pathlist != nullptr){
        delete pathlist;
        pathlist = nullptr;
    }
}

void Fox::playAudio(int a){
    auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.3){
        r = 0.4;
    }else if(r == 0.5){
        r = 0.4;
    }else if(r == 0.6){
        r = 0.55;
    }
    
    //std::cout<< "random Fox -> " << r << "\n";

    if(a==0){
        //Attack
        am->shootAudio("Voces/AI/Zorro/Atacar",0,r);
    }else if(a==1){
        //RDamage
        am->shootAudio("Voces/AI/Zorro/RDaño",0,r);
    }else if(a==2){
        //Die
        am->shootAudio("Voces/AI/Zorro/Morir",0,r);

    }

    am = nullptr;
}

Vector3f Fox::getLastPosition(){
    return lastpos;
}