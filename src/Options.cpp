#include "Options.h"
#include "Motor3D.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "Dungeon.h"
#include "Game.h"
#include "FactoriaNivel.h"

Options* Options::Instance(){
    static Options pinstance;
    return &pinstance;
}

void Options::handleInput(){
    
    auto* m = m3D::Motor3D::Instance();
    auto* e = EventManager::Instance();

    if(then > 0.1){

        bool connected = m->isconectedJoystick();

        float x,y;

        if(connected){
            m->initController();
            m->checkJoystickAxes(x,y);
        }

        then = 0;

        if(m->onKeyPressed('a')) e->addEvent(Event{Type::A_KEYBOARD_PRESSED});
        if(m->onKeyPressed('d')) e->addEvent(Event{Type::D_KEYBOARD_PRESSED});
        if(connected){
            if(x<=-0.3f) e->addEvent(Event{Type::A_KEYBOARD_PRESSED});
            if(x>=0.3f)  e->addEvent(Event{Type::D_KEYBOARD_PRESSED});
        }

        if(up){
            if(m->onKeyPressed('s')) e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
            if(m->onKeyPressed('w')) e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
            if(connected){
                if(y<=-0.3f)e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
                if(y>=0.3f)e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
            }
        }

        if(m->onKeyPressed('p')){
            e->addEvent(Event{Type::P_KEYBOARD_PRESSED});
        }else if(connected && m->onButtonPressed('B')){
            e->addEvent(Event{Type::P_KEYBOARD_PRESSED});
        }

    }else{
        then += m->getDeltaTime();
    }

    //Joystick - Controller
}

void Options::draw(){
    
    auto* m = m3D::Motor3D::Instance();

    if(!drawed || redraw){
        if(!drawed){
            m->activarBillboards(Estado::StateType::OPTIONS_STATE);
            drawed=true;
        }
        if(redraw){
            if(pulsed){
                m->redibujarOptions(option,option2);
            }else{
                m->redibujarOptions(option);
            }
            redraw = false;
        }
    }

    m->drawScene();
    m = nullptr;
}

void Options::update(){
    EventManager::Instance()->runManager();
    clock += m3D::Motor3D::Instance()->getDeltaTime();
}

void Options::clearState(){
}

Options::Options()
{
    drawed = false;
    redraw = true;
    option = 0;
    clock = 0;
    pulsed = false;
    fullscreen = false;
    resolution = 1;
    audiolevel = 10;
    audioon = true;
    option2 = 0;
    left = true;
    up = true;
}

void Options::setClock(float c){
    clock = c;
}

void Options::addEvents(){

    auto* e = EventManager::Instance();

    if(!pulsed){
        e->addListener(W_KEYBOARD_PRESSED, [&](){upOption();});
        e->addListener(S_KEYBOARD_PRESSED, [&](){downOption();});
    }
    e->addListener(A_KEYBOARD_PRESSED, [&](){leftOption();});
    e->addListener(D_KEYBOARD_PRESSED, [&](){rightOption();});
    e->addListener(P_KEYBOARD_PRESSED, [&](){selectOption();});

    e = nullptr;
}

void Options::exit(){
}


Options::~Options(){}

void Options::upOption(){

   // auto* m = m3D::Motor3D::Instance();

    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(!pulsed){
            if(option == 0 || option == 1){
                option = 3;
            }else{
                option--;
            }
        }else{
            option2 = 0;
            if(audiolevel != 10){
                audiolevel += 1;
            }
            if(audioon) AudioManager::Instance()->setVolume(audiolevel/10);
        }
        clock = 0;
        redraw = true;
    }
}

void Options::downOption(){
    //auto* m = m3D::Motor3D::Instance();

    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(!pulsed){
            if(option == 3){
                option = 0;
            }else if(option == 0){  
                option = 2;
            }else{
                option++;
            }
        }
        redraw = true;
        clock = 0;
    }
}

void Options::leftOption(){
    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(!pulsed){
            if(option == 0){
                option = 3;
            }else{
                option--;
            }
        }else{
            if(option == 1){
                option2 = 0;
                if(audiolevel != 0){
                    audiolevel -= 1;
                }
                if(audioon) AudioManager::Instance()->setVolume(audiolevel/10);
            }else if(option != 2){
                if(option2 == 0){
                    option2 = 1;
                }else{
                    option2 = 0;
                }
                
            }else{
                if(option2 == 0){
                    option2 = 4;
                }else{
                    option2 -= 1;
                }
            }
        }
        redraw = true;
        clock = 0;
    }
}

void Options::rightOption(){
    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
        if(!pulsed){
            if(option == 3){
                option = 0;
            }else{
                option++;
            }
        }else{
            if(option == 1){
                option2 = 1;
                if(audiolevel != 10){
                    audiolevel += 1;
                }
                if(audioon) AudioManager::Instance()->setVolume(audiolevel/10);
            }else if(option != 2){
                if(option2 == 1){
                    option2 = 0;
                }else{
                    option2 = 1;
                }
            }else{
                if(option2 == 4){
                    option2 = 0;
                }else{
                    option2 += 1;
                }
            }
        }
        redraw = true;
        clock = 0;
    }
}

void Options::selectOption(){
    auto* m = m3D::Motor3D::Instance();

    if(clock > 0.2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/SeleccionarOpcion",-1, 0);
        if(!pulsed){
            switch(option){
                case 0: //Audio On / Off
                    pulsed = true;
                    up = false;
                break;
                case 1: //Level audio
                    if(audioon){
                        pulsed = true;
                        up = false;
                    }
                break;
                case 2: //Resolution
                    pulsed = true;
                    up = false;
                break;
                case 3: //Return to pause
                    pulsed = false;
                    drawed = false;
                    redraw = true;
                    option = 0;
                    option2 = 0;
                    Game::Instance()->setState(Estado::StateType::PAUSE_MENU_STATE);
                    m->activarBillboards(Estado::StateType::PAUSE_MENU_STATE);
                break;
            }
        }else{
            switch(option){
                case 0: //Audio On / Off
                    if(option2 == 0){
                        AudioManager::Instance()->setVolume(audiolevel/10);
                        audioon = true;
                    }else{
                        AudioManager::Instance()->setVolume(0);
                        audioon = false;
                    }
                break;
                case 2: //Resolution
                    switch(option2){
                        case 0: //640-480
                            m->setWindowSize(640,480);
                        break;
                        case 1: //1080-720
                            m->setWindowSize(1080,720);
                        break;
                        case 2: //1280-720
                            m->setWindowSize(1280,720);
                        break;
                        case 3: //1600-1200
                            m->setWindowSize(1600,1200);
                        break;
                        case 4: //2048-1080
                            m->setWindowSize(2048,1080);
                        break;
                    }
                break;
            }
            up = true;
            pulsed = false;
            option2 = 0;
            redraw = true;
        }
        clock = 0;
    }
}

void Options::setDrawed(bool d){
    drawed = d;
}