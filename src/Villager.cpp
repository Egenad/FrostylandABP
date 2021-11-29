#include "Villager.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include "EventManager.h"
#include "Dialog_State.h"
#include "Player.h"
#include "AudioManager.h"
#include <iostream>

Villager::~Villager(){

}

//GETTERS

int Villager::getDialog(){
    return dialog;
}

int Villager::getMaxDialog(){
    return maxdialog;
}

int Villager::getIniDialog(){
    return inidialog;
}

int Villager::getState(){
    return state;
}

//SETTERS

void Villager::setDialog(int d){
    if(dialog + d > maxdialog){
        d = inidialog;
    }
    dialog = d;
    if(dialog == (inidialog + maxdialog)){
        setState(state+1);
        if(state == 1){
            setIniDialog(inidialog +1);
        }
    }
}

void Villager::resetVillager(){ 

    setDialog(getIniDialog());
    resetPosition();
}

void Villager::setMaxDialog(int m){
    maxdialog = m;
}

void Villager::setIniDialog(int i){
    inidialog = i;
}

void Villager::setState(int s){
    state = s;
}

//MAIN METHODS

void Villager::callDialog(int val){
    auto* d = Dialog_State::Instance();
    d->setParams(dialog, maxdialog, getType());
    Game::Instance()->setState(Estado::StateType::DIALOG_STATE);
}

void Villager::updatePEntities(){

        // ?¿
}

void Villager::playAudio(int a){
    auto* am = AudioManager::Instance();
    /*float r = ((double) rand() / (RAND_MAX));

    
    if(r == 0.3){
        r = 0.4;
    }else if(r == 0.5){
        r = 0.4;
    }else if(r == 0.6){
        r = 0.55;
    }
    */
    //std::cout<< "random Villager -> " << r << "\n";

    if(a==0){
        //Talk
       am->shootAudio("Voces/AI/Aldeano/Hablar",-1,0);
    }

    am = nullptr;
}
