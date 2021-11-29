#include "Slingshot.h"
#include "AudioManager.h"

Slingshot::Slingshot(){

	ps_x = 0;
	ps_y = 0;
	ps_z = 0;
	r = 5;

	damage = 1;
	hitRange = 100;
	attackVel = 2.f;
	attackAng = 3.14/4;
	
}

Slingshot::~Slingshot(){

}

void  Slingshot::updateEntity(){
	
}


void  Slingshot::playAudio(int a){
	auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.5){
        r = 0.4;
    }

    if(a==0){
        //Miss
        am->shoot3DAudio("HardFX/Objetos/Armas/Tirachinas/Lanzamiento",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==1){
        //Hit
        am->shoot3DAudio("HardFX/Objetos/Armas/Tirachinas/Impacto",0,r, Vector3f(ps_x, ps_y, ps_z));
    }

    am = nullptr;
}