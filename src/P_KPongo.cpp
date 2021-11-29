#include "P_KPongo.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include <iostream>
#include "Room.h"
#include "StrategyA.h"
#include "TaskTypes.h"
#include "AudioManager.h"

P_KPongo::P_KPongo(Vector3f pos){

    //Characteristics
	vida = 10;
    maxlife = 10;
    ataque = 5;
    velocidadX = 0.f;
    velocidadZ = 0.f;
    ps_x = pos.getX();
    ps_y = pos.getY();
    init_y = ps_y + 18;
    ps_z = pos.getZ();
    maxvel = 20.f;
    coins = 30;
    angle = 0;
    rleft = true;
    h_r = 100;
    s_r = 190;
    width = 60;
    height = 70;
    chaseando = false;
    salaActual = -1;
    death = false;
    state = 0;
    setMass(1000.0f);
    deactivated = true;
    strategy = new StrategyA();
    start = false;

    //Physics
    PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,22.5f,22.5f,22.0f,1000.f,rigidBody,phy_state,bodyShape);

    //--------Animation------------
    anim_state = 0;
    animationtime = 0.0;
    animationframe = 1;
    animation = false;
    a_time = -1;
}

P_KPongo::~P_KPongo(){

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
        delete strategy;
    }
}

/////////////////////////////////////////
// MAIN METHODS  
/////////////////////////////////////////

void P_KPongo::updateEntity(){

    if(!death){

    	auto* m = m3D::Motor3D::Instance();

        if(start){
            if(salaActual == -1){
                salaActual = Dungeon::Instance()->wichRoomIAmAt(ps_x, ps_z);
            }

            if(strategy){
                strategy->execute(getID());
            }

            if(rigidBody!=nullptr){
                PhysicsEngine::Instance()->moveEnemyLinear(rigidBody, deactivated, velocidadX, velocidadZ, ps_x, ps_y, ps_z);
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

            updateAnimation();

            m->setPositionEntity(soldier,ps_x,ps_y,ps_z);
            m->setPositionEntity(shadow,ps_x,init_y,ps_z);

            m = nullptr;
        }
    }
}

void P_KPongo::updateAnimation(){
    auto* m = Motor3D::Instance();

    if(animationtime > 0.2){

        animationtime = 0;

        if(anim_state == 0){
            m->setMeshTexture(soldier,"../assets/textures/kpongo.png");
        }else if(anim_state == 1){
            m->setMeshTexture(soldier,"../assets/textures/kpongodesliz.png");
        }else if(anim_state == 2){
            if(animationframe == 1 || animationframe == 3){
                m->setMeshTexture(soldier,"../assets/textures/kpongomareo1.png");
            }else if(animationframe == 2){
                m->setMeshTexture(soldier,"../assets/textures/kpongomareo2.png");
            }else if(animationframe == 4){
                m->setMeshTexture(soldier,"../assets/textures/kpongomareo3.png");
            }
            animationframe++;
            if(animationframe == 5) animationframe = 1;
        }else if(anim_state == 3){
            m->setMeshTexture(soldier,"../assets/textures/kpongocaida.png");
        }
    }else{
        animationtime += m->getDeltaTime();
    }
}

void P_KPongo::playAudio(int a){
    auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.3){
        r = 0.4;
    }else if(r == 0.5){
        r = 0.4;
    }else if(r == 0.6){
        r = 0.55;
    }
    
    //std::cout<< "random KPONGO -> " << r << "\n";
    
    if(a==0){
        //Attack
        am->shoot3DAudio("Voces/AI/KingPongo/Atacar",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==1){
        //RDamage
        am->shoot3DAudio("Voces/AI/KingPongo/RDaño",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==2){
        //Die
        am->shoot3DAudio("Voces/AI/KingPongo/Morir",0,r, Vector3f(ps_x, ps_y, ps_z));
        am->stop("Musica/Juego/Batalla");
        if(am->isPlaying("HardFX/KingPongo/Mareo")){
            am->stop("HardFX/KingPongo/Mareo");
        }
        am->shootAudio("Musica/Juego/Castillo2",-1,0);
    }else if(a==3){
        //Talk
        am->shoot3DAudio("Voces/AI/KingPongo/Hablar",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==4){
        //Laugh
        am->shoot3DAudio("Voces/AI/KingPongo/Reir",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==5){
        if(am->isPlaying("HardFX/KingPongo/Mareo") == false){
            am->shoot3DAudio("HardFX/KingPongo/Mareo",-1,0, Vector3f(ps_x, ps_y, ps_z));
        }
    }else if(a==6){
        am->shoot3DAudio("HardFX/KingPongo/Rebotar",-1,0, Vector3f(ps_x, ps_y, ps_z));
    }

    am = nullptr;
}

void P_KPongo::recalculatePath(){}

void P_KPongo::startBattle(){
    AudioManager::Instance()->shootAudio("Musica/Juego/Batalla", -1, 0);
    start = true;
}

void P_KPongo::setInitY(float f){
    init_y = f;
}