#include "Sword.h"
#include "AudioManager.h"

Sword::Sword(){

	ps_x = 0;
	ps_y = 0;
	ps_z = 0;
	r = 5;

	damage = 2;
	hitRange = 60;
	attackVel = 2.f;
	attackAng = 3.14/4;
	
}

Sword::~Sword(){

}

void Sword::updateEntity(){

}

void Sword::playAudio(int a){

	auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.5){
        r = 0.4;
    }

    SoundEvent* e;

    if(a==0){
        //Miss
        e = am->shoot3DAudio("HardFX/Objetos/Armas/Espada/NoImpacto",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==1){
        //Hit
        e = am->shoot3DAudio("HardFX/Objetos/Armas/Espada/Impacto",0,r, Vector3f(ps_x, ps_y, ps_z));
        e->setVolume(0.2);
    }

    am = nullptr;

}