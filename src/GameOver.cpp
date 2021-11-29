#include "GameOver.h"
#include "Menu.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include <iostream>
#include "EventManager.h"
#include "AudioManager.h"
#include "Game.h"
#include "Player.h"
#include "Globals.h"
#include "FactoriaNivel.h"

GameOver* GameOver::Instance(){
    static GameOver pinstance;
    return &pinstance;
}

GameOver::GameOver()
{
    drawed = false;
    option = 0;
    clock = 0;
    redraw = false;
}

void GameOver::handleInput(){
    auto* m = m3D::Motor3D::Instance();
    auto* e = EventManager::Instance();

    if(m->onKeyPressed('s')) e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
    if(m->onKeyPressed('w')) e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
    if(m->onKeyPressed('p')) e->addEvent(Event{Type::P_KEYBOARD_PRESSED});

    //Joystick - Controller
    if(m->isconectedJoystick()){
        m->initController();
        float x,y;
        m->checkJoystickAxes(x,y);
        if(y<=-0.5f)e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
        if(y>=0.5f)e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
        if(m->onButtonPressed('B'))e->addEvent(Event{Type::P_KEYBOARD_PRESSED});
    }
}

void GameOver::draw(){
	auto* m = m3D::Motor3D::Instance();
    if(!drawed || redraw){
        if(!drawed){
            //m->activarBillboards(Estado::StateType::GAME_OVER_STATE);
            m3D::Motor3D::Instance()->loadTextures(2);
            drawed = true;
        }
        if(redraw){
            m->dibujarMenuGameOver(option);
            redraw = false;
        }
    }

    m->drawScene();
    m = nullptr;
}

void GameOver::update(){
	EventManager::Instance()->runManager();
}

void GameOver::clearState(){

}

void GameOver::addEvents(){

    auto* e = EventManager::Instance();

    e->addListener(W_KEYBOARD_PRESSED, [&](){upOption();});
    e->addListener(S_KEYBOARD_PRESSED, [&](){downOption();});
    e->addListener(P_KEYBOARD_PRESSED, [&](){selectOption();});
    AudioManager::Instance()->shootAudio("Musica/GameOver", -1, 0);

    e = nullptr;

}

void GameOver::upOption(){

    auto* m = m3D::Motor3D::Instance();

    if(clock < m->getNow()){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(option == 0){
            option = 1;
        }else{
            option--;
        }
        clock = m->getNow() + 0.1;
        redraw = true;
    }
}

void GameOver::downOption(){
    auto* m = m3D::Motor3D::Instance();

    if(clock < m->getNow()){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(option == 1){
            option = 0;
        }else{
            option++;
        }
        clock = m->getNow() + 0.1;
        redraw = true;
    }
}

void GameOver::selectOption(){
    auto* m = m3D::Motor3D::Instance();

    if(clock < m->getNow()){
        switch(option){
            case 0:
                AudioManager::Instance()->shootAudio("Diseño/Menu/SeleccionarJugar",-1, 0);
                resetGame();
            break;
            case 1:
                AudioManager::Instance()->shootAudio("Diseño/Menu/SeleccionarSalir",-1, 0);
                Game::Instance()->setState(Estado::StateType::MENU_STATE);
                getToMenu();
            break;
        }
        clock = m->getNow() + 0.1;
    }
}

void GameOver::resetGame(){
    auto* g = Game::Instance();
    drawed = false;
    g->setState(Estado::StateType::DUNGEON_STATE);
    //g->setCloseAll(true);
    Dungeon::Instance()->setChangeTextures(true);
    auto* d = Dungeon::Instance();
    d->setLevel(0);
    d->setLevel(1);
    d->setResetPlayer();
    d->setLoaded(false);
    option = 0;
    Dungeon::Instance()->setGameOver(false);
    AudioManager::Instance()->stop("Musica/GameOver");

}

void GameOver::getToMenu(){
    
    auto* m = Menu::Instance();
    auto* d = Dungeon::Instance();
    Game::Instance()->setClose(false);
    //Game::Instance()->setCloseAll(true);
    m->setClock(m3D::Motor3D::Instance()->getNow() + 0.2);
    m->setDrawed(false);
    d->clearState();
    d->setResetPlayer();
    d->setLoaded(false);
    AudioManager::Instance()->stop("Musica/GameOver");
    //Player::Instance()->clearInstance();
    drawed=false;
    option=0;
    Dungeon::Instance()->setGameOver(false);

}

GameOver::~GameOver(){}
