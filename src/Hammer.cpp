#include "Hammer.h"
#include "AudioManager.h"

Hammer::Hammer(){

	ps_x = 0;
	ps_y = 0;
	ps_z = 0;
	r = 5;

	damage = 3;
	hitRange = 30;
	attackVel = 2.f;
	attackAng = 3.14/4;

}

Hammer::~Hammer(){

}

void  Hammer::updateEntity(){
	
}

void  Hammer::playAudio(int a){
	auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.5){
        r = 0.4;
    }

    //std::cout<< "random -> " << r << "\n";

    if(a==0){
        //Miss
        am->shoot3DAudio("HardFX/Objetos/Armas/Martillo/ImpactoSuelo",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==1){
        //Hit
        am->shoot3DAudio("HardFX/Objetos/Armas/Martillo/ImpactoPersonaje",0,r, Vector3f(ps_x, ps_y, ps_z));
    }

    am = nullptr;
}