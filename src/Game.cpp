#include "Game.h"
#include "Dungeon.h"
#include "GameOver.h"
#include "Options.h"
#include "Menu.h"
#include "Pause.h"
#include "Motor3D.h"
#include "EventManager.h"
#include "Player.h"
#include "Item_Selector.h"
#include "Inventory.h"
#include "AudioManager.h"
#include "FactoriaNivel.h"
#include "PhysicsEngine.h"
#include "Dialog_State.h"

#define UPDATE_TICK_TIME 1000/120

Game* Game::Instance(){
    static Game pinstance;
    return &pinstance;
}

void Game::setState(Estado::StateType type){
    
    EventManager::Instance()->cleanMap();

    switch(type){
        case Estado::StateType::MENU_STATE:
            state = Menu::Instance();
            break;
        case Estado::StateType::DUNGEON_STATE:
            closeall = true;
            state = Dungeon::Instance();
            break;
        case Estado::StateType::GAME_OVER_STATE:
            state = GameOver::Instance();
            break;
        case Estado::StateType::INVENTORY_STATE:
            state = Inventory::Instance();
            break;
        case Estado::StateType::DIALOG_STATE:
            state = Dialog_State::Instance();
            break;
        case Estado::StateType::ITEM_SELECTOR_STATE:
            state = Item_Selector::Instance();
            break;
        case Estado::StateType::PAUSE_MENU_STATE:
            state = Pause::Instance();
            break;
        case Estado::StateType::OPTIONS_STATE:
            state = Options::Instance();
            break;
        default:
            state = Menu::Instance();
            break;
    }

    state->addEvents();
}

void Game::runGame(){
    
    auto* m = m3D::Motor3D::Instance();
    auto* audio = AudioManager::Instance();

    int active = m->sigueActivo();

    interpolation = 0;

    while(active == 1 || active == 2){
        if(active == 2){ //juego abierto y ventana activa
            state->handleInput();

            //interpolation = m->getElapsedTime() * 1000; //segundos

            //if(interpolation >= UPDATE_TICK_TIME){
                state->update();
              //  m->resetFrameDelta();
            //}

            state->draw();
        }
        active = m->sigueActivo();
        if(close == true){
            active = 0;
        }
        //Update audio
        audio->update();
    }

    m = nullptr;
    audio = nullptr;
    clearGame();
}

Game::Game() {
    closeall = false;
}

Game::Game(const Game& orig) {
}

void Game::clearGame(){
    //Borro el espacio de memoria del estado en el que me encuentro
    if(closeall){
        Dungeon::Instance()->clearState();
        PhysicsEngine::Instance()->clearKinematic();
        //GameOver::Instance()->clearState();
        //Player::Instance()->clearInstance();
        FactoriaNivel::Instance()->clearInstance();
    }

    EventManager::Instance()->clearInstance();
    AudioManager::Instance()->clearInstance();
    m3D::Motor3D::Instance()->clearInstance();
    Menu::Instance()->clearState();
    state = nullptr;
}

Game::~Game() {

}

void Game::setClose(bool sn){
    close = sn;
}

void Game::setCloseAll(bool s){
    closeall = s;
}