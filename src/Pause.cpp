#include "Pause.h"
#include "Motor3D.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "Dungeon.h"
#include "Game.h"
#include "FactoriaNivel.h"
#include "Menu.h"
#include "PhysicsEngine.h"
#include "Player.h"


Pause* Pause::Instance(){
    static Pause pinstance;
    return &pinstance;
}

void Pause::handleInput(){

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

void Pause::draw(){
    
    auto* m = m3D::Motor3D::Instance();

    m->beginScene(true, true, 0, 200, 200, 200);
    
    if(!drawed || redraw){
        if(!drawed){
            m->activarBillboards(Estado::StateType::PAUSE_MENU_STATE);
            drawed = true;
        }
        if(redraw){
            m->dibujarMenuPause(option);
            redraw = false;
        }
    }

    m->drawScene();
    m = nullptr;
}

void Pause::update(){
    EventManager::Instance()->runManager();
    clock += m3D::Motor3D::Instance()->getDeltaTime();
}

void Pause::clearState(){
}

Pause::Pause(){
    drawed = false;
    option = 0;
    clock = 0;
    redraw = false;
}

void Pause::addEvents(){

    auto* e = EventManager::Instance();

    e->addListener(W_KEYBOARD_PRESSED, [&](){upOption();});
    e->addListener(S_KEYBOARD_PRESSED, [&](){downOption();});
    e->addListener(P_KEYBOARD_PRESSED, [&](){selectOption();});

    e = nullptr;
}

void Pause::changeState(){

    drawed = false;
    
    Game::Instance()->setState(DUNGEON_STATE);
}

Pause::~Pause(){

}

void Pause::upOption(){

    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(option == 0){
            option = 3;
        }else{
            option--;
        }
        clock = 0;
        redraw = true;
    }
}

void Pause::downOption(){

    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(option == 3){
            option = 0;
        }else{
            option++;
        }
        clock = 0;
        redraw = true;
    }
}

void Pause::selectOption(){
    auto* m = m3D::Motor3D::Instance();

    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/SeleccionarOpcion",-1, 0);
        switch(option){
            case 0:
                drawed = false;
                Game::Instance()->setState(Estado::StateType::DUNGEON_STATE);
                m->activarBillboards(Estado::StateType::DUNGEON_STATE);
            break;
            case 1:
                FactoriaNivel::Instance()->saveGame(Dungeon::Instance()->getLevel());
            break;
            case 2:
                Game::Instance()->setState(Estado::StateType::OPTIONS_STATE);
            break;
            case 3:
                Game::Instance()->setState(Estado::StateType::MENU_STATE);
                getToMenu();
            break;
        }
        clock = 0;
    }
}

void Pause::getToMenu(){

    auto* m = Menu::Instance();
    auto* d = Dungeon::Instance();
    Game::Instance()->setClose(false);
    Game::Instance()->setCloseAll(true);
    m->setClock(m3D::Motor3D::Instance()->getNow() + 0.2);
    m->setDrawed(false);
    d->clearState();
    d->setResetPlayer();
    d->setLoaded(false);
    AudioManager::Instance()->stop("Musica/Juego/Castillo2");
    AudioManager::Instance()->stop("Musica/Juego/Batalla");
    Player::Instance()->clearInstance();
    drawed = false;
    option = 0;
}