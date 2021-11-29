#include "Menu.h"
#include "Motor3D.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "Dungeon.h"
#include "Game.h"
#include "FactoriaNivel.h"

Menu* Menu::Instance(){
    static Menu pinstance;
    return &pinstance;
}

void Menu::handleInput(){
    
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
        if(y<=-0.3f)e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
        if(y>=0.3f)e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
        if(m->onButtonPressed('B'))e->addEvent(Event{Type::P_KEYBOARD_PRESSED});
    }
}

void Menu::draw(){
    
    auto* m = m3D::Motor3D::Instance();

    if(!drawed || redraw){
        if(!drawed){
            m->loadTextures(0);
        }
        m->dibujarMenu(option);
        drawed=true;
    }

    m->drawScene();
    m = nullptr;
}

void Menu::update(){
    EventManager::Instance()->runManager();
}

void Menu::clearState(){
}

Menu::Menu()
{
    drawed = false;
    redraw = false;
    option = 0;
    clock = 0;
}

void Menu::setClock(float c){
    clock = c;
}

void Menu::addEvents(){

    auto* e = EventManager::Instance();
    auto* am = AudioManager::Instance();

    e->addListener(W_KEYBOARD_PRESSED, [&](){upOption();});
    e->addListener(S_KEYBOARD_PRESSED, [&](){downOption();});
    e->addListener(P_KEYBOARD_PRESSED, [&](){selectOption();});
    am->shootAudio("Musica/Menu", -1, 0);
    am->shootAudio("Ambientes/Juego/Nieve", -1, 0);

    e = nullptr;
    am = nullptr;
}

void Menu::exit(){
    Game::Instance()->setClose(true);
}


Menu::~Menu(){}

void Menu::upOption(){

    auto* m = m3D::Motor3D::Instance();

    if(clock < m->getNow()){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(option == 0){
            option = 2;
        }else{
            option--;
        }
        clock = m->getNow() + 0.2;
        redraw = true;
    }
}

void Menu::downOption(){
    auto* m = m3D::Motor3D::Instance();

    if(clock < m->getNow()){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(option == 2){
            option = 0;
        }else{
            option++;
        }
        clock = m->getNow() + 0.2;
        redraw = true;
    }
}

void Menu::selectOption(){
    auto* m = m3D::Motor3D::Instance();
    auto* am = AudioManager::Instance();

    if(clock < m->getNow()){
        switch(option){
            case 0:
            if(FactoriaNivel::Instance()->saveDataAvailable()){
                am->shootAudio("Diseño/Menu/SeleccionarJugar",-1, 0);
                Game::Instance()->setState(Estado::StateType::DUNGEON_STATE);
                auto* d = Dungeon::Instance();
                d->setLoadSaveData(true);
                d->setChangeTextures(true);
                am->stop("Musica/Menu");
                am->stop("Ambientes/Juego/Nieve");
            }
            break;
            case 1:
                am->shootAudio("Diseño/Menu/SeleccionarJugar",-1, 0);
                resetGame();
                am->stop("Ambientes/Juego/Nieve");
                am->stop("Musica/Menu");
            break;

            case 2:
                am->shootAudio("Diseño/Menu/SeleccionarSalir",-1, 0);
                exit();
            break;
        }
        clock = m->getNow() + 0.1;
    }

    m = nullptr;
    am = nullptr;
}

void Menu::setDrawed(bool d){
    drawed = d;
}

void Menu::resetGame(){
    auto* g = Game::Instance();
    drawed = false;
    g->setState(Estado::StateType::DUNGEON_STATE);
    g->setCloseAll(true);
    //m3D::Motor3D::Instance()->insertLifes();
    Dungeon::Instance()->setChangeTextures(true);
    auto* d = Dungeon::Instance();
    d->setLevel(0);
    d->setLevel(1);
    option = 0;
    FactoriaNivel::Instance()->destroySaveData();

}
