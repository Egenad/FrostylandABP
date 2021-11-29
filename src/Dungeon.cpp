#include "Dungeon.h"
#include "Motor3D.h"
#include "GameOver.h"
#include "Game.h"
#include "Player.h"
#include "EventManager.h"
#include "Event.h"
#include "Interactible.h"
#include "PhysicsEngine.h"
#include "Globals.h"
#include "FactoriaNivel.h"
#include "Chest.h"
#include "P_Soldier.h"
#include "P_KPongo.h"
#include "Seal_Normal.h"
#include "Trigger_Camera.h"
#include "Dialog_State.h"
#include "AudioManager.h"

using namespace m3D;

Dungeon* Dungeon::Instance(){
    static Dungeon pinstance;
    return &pinstance;
}

Dungeon::Dungeon()
{
    //ctor
    e_released = true;
    erased = false;
    loaded = false;
    loadsave = false;
    loadlevel = false;
    debug = false;
    IAdebug = false;
    level = 1;
    changeTextures = true;
    lastlevel = 0;
    resetPlayer = true;
    levelmediaY = 0;
    dungeonGraph = nullptr;
    init_boss_animation = false;
    objrec = 6;
    transition = false;
    radius = 3.3;
    tnumber = 0;
    mt_up = false;
    gameOver = false;
    fps = 0;
    fps2 = 0;
    fps_clock = 0;
    enemyselec = 0;
}

void Dungeon::loadDungeon(){

    PhysicsEngine::Instance()->initPhysics(level);
    auto* m = Motor3D::Instance();
    auto* pl = Player::Instance();
    auto* fn = FactoriaNivel::Instance();

    m->drawLoadingScreen();

    if(resetPlayer){ 
        pl->setType(PLAYER_ENTITY);
        pl->initPlayer();
        m->drawBillBoardEntity(pl->getNode(), pl->getShadow(), pl->getX(), pl->getY(), pl->getZ(), pl->getType());
        fn->putPlayerPosition(level, lastlevel);
        pl->createPlayerKinematic();
        AudioManager::Instance()->shootAudio("Musica/Juego/Castillo2", 2, 12);
        resetPlayer = false;
    }else{
        fn->putPlayerPosition(level, lastlevel);
        pl->createPlayerKinematic();
    }

    m->drawLoadingScreen();

    then = m->getNow() + 0.1;
    //Inicializo dungeon y creo al jugador*/
    m->inicializarDungeon(level);
    //Dibujo al jugador
    //pl->setPositionPhysic();
    pl->resetCameraPosition();

    m->drawLoadingScreen();

    //Create the rooms the dungeon is compositted of
    std::string ints = std::to_string(level);
    std::string tolevel = "Level" + ints;
    fn->readLevel(tolevel.c_str(),rooms,doors,enemies,seals);
    //We draw all NPC's and Doors the dungeon has
    if(loadlevel){
        fn->loadGame(level,rooms,doors,enemies, seals);
        AudioManager::Instance()->setParameterValue("Musica/Juego/Castillo2", 1, level);
        loadlevel = false;
    }

    m->drawLoadingScreen();
    /*
    if(loadsave){
        fn->loadGame(level,rooms,doors,enemies);
        fn->loadPlayer();
        loadsave = false;
    }
    */
    for(long unsigned int i=0; i<enemies.size(); i++){
        auto* en = enemies[i];
        m->drawBillBoardEntity(en->getNode(), en->getShadow(), en->getX(), en->getY(), en->getZ(), en->getType());
        en = nullptr;
    }

    m->drawLoadingScreen();

    for(long unsigned int i=0; i<seals.size(); i++){
        auto* se = seals[i];
        m->drawBillBoardEntity(se->getNode(), se->getShadow(), se->getX(), se->getY(), se->getZ(), se->getType());
        se = nullptr;
    }

    m->drawLoadingScreen();

    for(long unsigned int i=0; i<doors.size(); i++){
        doors[i]->drawDoor();
    }

    m->drawLoadingScreen();

    for(long unsigned int i=0; i<rooms.size(); i++){
        rooms[i]->drawInteractuables();
    }

    m->drawLoadingScreen();

    init_boss_animation = false;
    animation_clock = 0.f;

    m = nullptr;
    fn = nullptr;
    pl = nullptr;

}

void Dungeon::handleInput(){
    auto* f = PhysicsEngine::Instance();
    auto* m = Motor3D::Instance();
    auto* e = EventManager::Instance();
    bool controller=false;

    if(loaded){

        f->initMovePlayer();

         //Joystick - Controller
        float x=0,y=0;
        if(m->isconectedJoystick()){
            controller = m->isconectedJoystick();
            m->initController();
            m->checkJoystickAxes(x,y);
        }

        if(!transition){
            if(m->onKeyPressed('a') || (controller && x<=-0.3f)) e->addEvent(Event{Type::A_KEYBOARD_PRESSED});
            if(m->onKeyPressed('s') || (controller && y>=0.3f)) e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
            if(m->onKeyPressed('d') || (controller && x>=0.3f)) e->addEvent(Event{Type::D_KEYBOARD_PRESSED});
            if(m->onKeyPressed('w') || (controller && y<=-0.3f)) e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
        
            if(then > 0.1){
                if(m->onKeyPressed('-') || (controller && m->onButtonPressed('-'))){ e->addEvent(Event{Type::ESCAPE_KEYBOARD_PRESSED}); then = 0;}
                if(m->onKeyPressed('o') || (controller && m->onButtonPressed('A'))){ e->addEvent(Event{Type::O_KEYBOARD_PRESSED}); then = 0;} 
                if(m->onKeyPressed('5')){ e->addEvent(Event{Type::M_KEYBOARD_PRESSED}); then = 0;}
                if(m->onKeyPressed('2')){ e->addEvent(Event{Type::I_KEYBOARD_PRESSED}); then = 0;}
                if(m->onKeyPressed('p') || (controller && m->onButtonPressed('B'))){e->addEvent(Event{Type::P_KEYBOARD_PRESSED}); then = 0;}
                if(m->onKeyPressed('4')){f->activeDebugDrawer(); then = 0;}
                if(m->onKeyPressed('1')){
                    if(getLevel() < 4){
                        Player::Instance()->setPosition(Vector3f(2500.f , 2500.f, 2500.f));    
                    }
                    then = 0;
                }   
                if(m->onKeyPressed('3')){
                    if(IAdebug == false){
                        IAdebug = true;
                        m->setDebugIA(true);
                    }else{
                        IAdebug = false;
                        m->setDebugIA(false);
                    }
                    then = 0;
                }   

                if(m->onKeyPressed('6')){
                    std::vector<int> enmis = getEnemiesPerRoomID(Player::Instance()->getSalaActual());
                    if(enemyselec + 1  < enmis.size()){
                        enemyselec++;
                    }else{
                        enemyselec = 0;
                    }
                    then = 0;
                }

                if(e_released){
                    if(m->onKeyPressed('i') || (controller && m->onButtonPressed('Y'))){
                        e_released=false;
                        then = 0;
                        erased=false;
                        e->addEvent(Event{Type::E_KEYBOARD_PRESSED});
                    }
                }else{
                    if(controller){
                        if(!m->onKeyPressed('i') && !m->onButtonPressed('Y')){
                            e_released=true;
                        }
                    }else if(!m->onKeyPressed('i')){
                        e_released = true;
                    }
                }

                then = 0;
            }else{
                then += m->getDeltaTime();
            }
        }
        
        f->setMovePlayer();
    }
    m = nullptr;
    e = nullptr;
}

void Dungeon::draw(){
    m3D::Motor3D::Instance()->drawScene();
}

void Dungeon::update(){

    auto* e = EventManager::Instance();
    auto* m = Motor3D::Instance();
    auto* pl = Player::Instance();
    auto* f = PhysicsEngine::Instance();

    e->runManager();

    if(!gameOver){
        if(!loaded){
            if(changeTextures){
                m->loadTextures(1);
                changeTextures = false;
            }else{
                m->drawLoadingScreen();
                m->clearDungeonMesh();
            }
            loadDungeon();
            loaded = true;
            m->setLoadingInactive(true);
        }else{
            if(!transition){
                pl->updateEntity();
                f->updatePhysics(Motor3D::Instance()->getDeltaTime());

                for(long unsigned int i=0; i<enemies.size(); i++){
                    enemies[i]->updateEntity();
                }

                for(long unsigned int i=0; i<dragboxes.size();i++){
                    dragboxes[i]->moveDragBox();
                }

                int rid = wichRoomIAmAt(pl->getX(), pl->getZ());
                bool updatedR = false;

                for(long unsigned int i=0; i<rooms.size() && !updatedR; i++){
                    if(rooms[i]->getID() == rid){
                        rooms[i]->updateEntity();
                        updatedR = true;
                    }
                }
    	        if(level == 1){
                    bool aux = false;
                    for(long unsigned int i=0; i<rooms.size() && !aux; i++){
                        if(rooms[i]->getID() == rid){
                            aux = true;
                            if(rooms[i]->getTable() != nullptr){
                                Decoration* d = rooms[i]->getTable();
                                setLOD(d, 200);
                            }
                        }
                    }
                }else if(level == 3){
                    bool aux = false;
                    for(long unsigned int i=0; i<rooms.size() && !aux; i++){
                        if(rooms[i]->getID() == rid){
                            if(rooms[i]->getStatue1() != nullptr){
                                Decoration* d = rooms[i]->getStatue1();
                                setLOD(d, 350);
                                d = rooms[i]->getStatue2();
                                setLOD(d,350);
                            }
                            aux = true;
                        }
                    }
                }
                if(IAdebug){
                    m->drawDatosPlayer();
                    fps2++;
                    if(fps_clock >= 1){
                        fps = fps2;
                        fps2 = 0;
                        fps_clock = 0;
                    }else{
                        fps_clock += m->getDeltaTime();
                    }
                    std::vector<int> enmis = getEnemiesPerRoomID(pl->getSalaActual());
                    if(enmis.size() != 0){
                        m->setDebugEnemy(true);
                        
                        m->drawDatosEnemy(enmis[enemyselec]);
                    }else{
                        m->setDebugEnemy(false);
                    }
                }
                for(long unsigned int i=0; i<doors.size(); i++){
                    doors[i]->updateEntity();
                }
                checkObjRec();  
            }else{

                for(long unsigned int i=0; i<doors.size(); i++){
                    doors[i]->updateEntity();
                }

                if(!init_boss_animation){
                    if(tnumber == 1){
                        if(radius <= 0){
                            transition = false;
                            //m->setTransition(false);
                            m->setRadius(0);
                            animation_clock = 0;
                            e->addEvent(START_CHANGE);
                        }else{
                            if(animation_clock > 0.01){
                                radius = radius - 0.1;
                                radius = roundf(radius * 10) / 10;
                                m->setRadius(radius);
                                animation_clock = 0;
                            }else{
                                animation_clock += m->getDeltaTime();
                            }
                        }
                    }else if(tnumber == 2){
                        if(radius >= 3.3){
                            transition = false;
                            m->setTransition(false);
                            animation_clock = 0;
                        }else{
                            if(animation_clock > 0.01){
                                if(radius != 0){
                                    radius = radius + 0.1;
                                }else{
                                    radius = 0.1;
                                }
                                m->setRadius(radius);
                                animation_clock = 0;
                            }else{
                                animation_clock += m->getDeltaTime();
                            }
                        }
                    }
                }else{
                    if(animation_clock > 0.5){
                        startTransition();
                        init_boss_animation = false;
                    }else{
                        animation_clock += m->getDeltaTime();
                    }
                }

                if(mt_up){
                    pl->moveUp();
                }else{
                    pl->moveDown();
                }
                pl->updateEntity();
                f->updatePhysics(Motor3D::Instance()->getDeltaTime());
            }    
        }
    }
}

void Dungeon::checkObjRec(){

    auto* m = m3D::Motor3D::Instance();
    if(objrec > 5){
        m->drawReceived(-1);
        objrec = 6;
    }else if(objrec != 6){
        objrec += m->getDeltaTime();
    }
}

void Dungeon::startObjRec(){
    objrec = 0;
}

void Dungeon::addEvents(){
    //GAME OVER
    auto* p = EventManager::Instance();
    p->addListener(GAME_OVER_CHANGE, [&](){
        m3D::Motor3D::Instance()->setDebugIA(false);
        IAdebug = false;
        Dungeon::Instance()->clearState();
        Player::Instance()->clearInstance();
        PhysicsEngine::Instance()->clearKinematic();
        Game::Instance()->setState(Estado::StateType::GAME_OVER_STATE);
        Dungeon::Instance()->setGameOver(true);
    });
    
    //Inputs
    p->addListener(A_KEYBOARD_PRESSED, [&](){Player::Instance()->moveLeft();});
    p->addListener(S_KEYBOARD_PRESSED, [&](){Player::Instance()->moveDown();});
    p->addListener(W_KEYBOARD_PRESSED, [&](){Player::Instance()->moveUp();});
    p->addListener(D_KEYBOARD_PRESSED, [&](){Player::Instance()->moveRight();});
    p->addListener(O_KEYBOARD_PRESSED, [&](){Player::Instance()->attack();});
    p->addListener(ESCAPE_KEYBOARD_PRESSED, [&](){Game::Instance()->setState(Estado::StateType::PAUSE_MENU_STATE);});
    p->addListener(P_KEYBOARD_PRESSED, [&](){Player::Instance()->interactObject();});
    p->addListener(E_KEYBOARD_PRESSED, [&](){openInventory();});
    //p->addListener(M_KEYBOARD_PRESSED, [&](){drawDebug();});
    p->addListener(M_KEYBOARD_PRESSED, [&](){Player::Instance()->heal(2);});
    p->addListener(I_KEYBOARD_PRESSED, [&](){Player::Instance()->fullInventory();});

    //Triggers
    p->addListener(LOCK_DOORS, [&](){lockDoors(true);});
    p->addListener(CREATE_ENEMIES, [&](){createEnemies();});
    p->addListener(UNLOCK_DOORS, [&](){lockDoors(false);});
    p->addListener(DELETE_ENEMIES, [&](){deleteEnemiesInPlayerRoom();});
    p->addListener(CREATE_CHEST_NORMAL_KEY, [&](){createChest(CHEST_NORMAL_KEY_ENTITY);});
    p->addListener(CREATE_CHEST_LEVEL_KEY, [&](){createChest(CHEST_LEVEL_KEY_ENTITY);});
    p->addListener(CREATE_CHEST_JAIL_KEY, [&](){createChest(CHEST_JAIL_KEY_ENTITY);});
    p->addListener(CREATE_CHEST_BOSS_KEY, [&](){createChest(CHEST_BOSS_KEY_ENTITY);});
    p->addListener(CREATE_CHEST_HAMMER, [&](){createChest(CHEST_HAMMER_ENTITY);});
    p->addListener(CREATE_CHEST_SLINGSHOT, [&](){createChest(CHEST_SLINGSHOT_ENTITY);});
    p->addListener(CREATE_CHEST_FOOD_BERRY, [&](){createChest(CHEST_FOOD_BERRY_ENTITY);});
    p->addListener(CREATE_CHEST_FOOD_MEAT, [&](){createChest(CHEST_FOOD_MEAT_ENTITY);});
    p->addListener(CREATE_CHEST_FOOD_FISH, [&](){createChest(CHEST_FOOD_FISH_ENTITY);});
    p->addListener(CREATE_CHEST_FOOD_ICE, [&](){createChest(CHEST_FOOD_ICE_ENTITY);});
    p->addListener(CREATE_QUEEN, [&](){createQueen();});
    p->addListener(DOOR_1_2_PRESSED, [&](){lockOneDoor(PLATE_1_2_INTERACTIBLE,false);});
    p->addListener(DOOR_1_2_RELEASED, [&](){lockOneDoor(PLATE_1_2_INTERACTIBLE,true);});
    p->addListener(PLAYER_FALLS, [&](){playerFalls();});
    p->addListener(CAMERA_ZOOM_IN, [&](){Player::Instance()->setCState(1);});
    p->addListener(CAMERA_ZOOM_OUT, [&](){Player::Instance()->setCState(3);});
    p->addListener(CHANGE_LEVEL, [&](){startTransition();});
    p->addListener(START_CHANGE, [&](){changeLevel();});

    //Boss related
    p->addListener(BOSS_DOOR_ANIMATION_START, [&](){setBossAnimation(true);});
    p->addListener(BOSS_DOOR_ANIMATION_END, [&](){startBossBattle();});
}

void Dungeon::startTransition(){
    auto* m = m3D::Motor3D::Instance();
    m->setTransition(true);
    m->setRadius(3.3);
    transition = true;
    radius = 3.3;
    tnumber = 1;
    animation_clock = 0;
    if(init_boss_animation){
        lastlevel = level;
        level++;
    }
    if(level > lastlevel){
        mt_up = true;
    }else{
        mt_up = false;
    }
}

void Dungeon::addEnemy(Enemy* e){
    enemies.push_back(e);
}

void Dungeon::addVillager(Villager* v){
    seals.push_back(v);
}

void Dungeon::addDragBox(DragBox* d){
    dragboxes.push_back(d);
}

void Dungeon::deleteEnemiesInPlayerRoom(){
    auto* pl = Player::Instance();
    int idr = wichRoomIAmAt(pl->getX(), pl->getZ());

    std::vector<int> enemies2 = getEnemiesPerRoomID(idr);

    int aux = enemies2.size();

    if(aux > 0){
        for(long unsigned int j=0; j<enemies.size(); j++){
            for(int i=0; i<aux; i++){
                Enemy* e = getEnemyPerID(enemies2[i]);
                if(e != nullptr && enemies[j] != nullptr){
                    if(e->getID() == enemies[j]->getID()){
                        delete enemies[j];
                        enemies.erase(enemies.begin()+j);
                        enemies2.erase(enemies2.begin()+i);
                    }
                }
            }
        }
    }
}

void Dungeon::createChest(EntityType type){
    auto* pl = Player::Instance();
    int idr = wichRoomIAmAt(pl->getX(), pl->getZ());
    Room* room = getRoomPerID(idr);
    if(room){
        Interactible* chest = new Chest(room->getCenterX(),levelmediaY,room->getCenterZ(),type, 1);
        room->addinteractible(chest);
        m3D::Motor3D::Instance()->drawChest(chest->getNode(), chest->getX(), chest->getY(), chest->getZ());
    }
}

void Dungeon::createQueen(){
    auto* pl = Player::Instance();
    int idr = wichRoomIAmAt(pl->getX(), pl->getZ());
    Room* room = getRoomPerID(idr);
    if(room){
        Queen* q = new Queen(Vector3f(room->getCenterX(), -10, room->getCenterZ()), 21, 2);
        addVillager(q);
        m3D::Motor3D::Instance()->drawBillBoardEntity(q->getNode(), q->getShadow(), q->getX(), q->getY(), q->getZ(), q->getType());
    }
}

void Dungeon::openInventory(){
    auto* am = AudioManager::Instance();

    /*
    if(am->isPlaying("Voces/AI/KingPongo/Mareo")){
        am->stop("Voces/AI/KingPongo/Mareo");
    }
    */
    float r = 0;
    float v = am->getVolume();
    if(v != 0){
        r = v / 2;
    }
    am->setVolume(r);
    am->shootAudio("Foleys/Inventario/Abrir", -1, 0);

    m3D::Motor3D::Instance()->setDebugIA(false);
    IAdebug = false;
    AudioManager::Instance()->shootAudio("Foleys/Inventario/Abrir", -1, 0);
    Game::Instance()->setState(Estado::StateType::INVENTORY_STATE);
    am = nullptr;
}

void Dungeon::createEnemies(){

    auto* pl = Player::Instance();
    int aux = wichRoomIAmAt(pl->getX(), pl->getZ());
    Room* r = getRoomPerID(aux);
    P_Soldier* p = new P_Soldier(Vector3f(r->getCenterX(), levelmediaY + 25, r->getCenterZ()));
    p->setType(P_SOLDIER_ENTITY);
    addEnemy(p);
    m3D::Motor3D::Instance()->drawBillBoardEntity(p->getNode(), p->getShadow(), p->getX(), p->getY(), p->getZ(), p->getType());
}

void Dungeon::createVillagers(){

    auto* pl = Player::Instance();
    int aux = wichRoomIAmAt(pl->getX(), pl->getZ());
    Room* r = getRoomPerID(aux);
    Seal_Normal* s = new Seal_Normal(Vector3f(r->getCenterX(), 16.f, r->getCenterZ()), 0, 1);
    s->setType(S_NORMAL_ENTITY);
    addVillager(s);
    m3D::Motor3D::Instance()->drawBillBoardEntity(s->getNode(), s->getShadow(), s->getX(), s->getY(), s->getZ(), s->getType());
}

void Dungeon::lockDoors(bool f){ 
    auto* pl = Player::Instance();
    int aux = wichRoomIAmAt(pl->getX(), pl->getZ());
    
    for(long unsigned int i = 0; i < doors.size(); i++){
        if(doors[i] != nullptr){
            if(doors[i]->getID1() == aux || doors[i]->getID2() == aux){
                doors[i]->setClosed(f);
                doors[i]->setLocked(f);
            }
        }
    }
}

void Dungeon::lockOneDoor(EntityType t, bool f){
    bool done = false;
    if(t == PLATE_1_2_INTERACTIBLE){
        for(long unsigned int i = 0; i < doors.size() && !done; i++){
            if(doors[i] != nullptr){
                if(doors[i]->getINDEX1() == 1 && doors[i]->getINDEX2() == 2 && !doors[i]->getInamovible()){  
                    doors[i]->setClosed(f);
                    doors[i]->setLocked(f);
                    done = true;
                }
            }
        }
    }
}

void Dungeon::playerFalls(){
    auto* pl = Player::Instance();
    pl->receiveDamage(2);

    bool done = false;

    int lastSala = pl->getLastSala();
    int salaActual = pl->getSalaActual();
    
    for(long unsigned int i=0; i<doors.size() && !done; i++){
        int aux1 = doors[i]->getID1();
        int aux2 = doors[i]->getID2();
        if((aux1 == lastSala && aux2 == salaActual) || (aux1 == salaActual && aux2 == lastSala)){
            done = true;
            float _x = doors[i]->getX();
            float _z = doors[i]->getZ();
            float aux2 = levelmediaY + 15.0;
            PhysicsEngine::Instance()->setPlayerPosition(_x,aux2,_z);
            pl->setPosition(Vector3f(_x, aux2, _z));
            pl->setIsMoving(true);
        }
    }
}

std::vector<Door*>& Dungeon::getDoors(){
    return doors;
}

std::vector<Enemy*>& Dungeon::getEnemies(){
    return enemies;
}

std::vector<Villager*>& Dungeon::getSeals(){
    return seals;
}

std::vector<Room*>& Dungeon::getRooms(){
    return rooms;
}

Door* Dungeon::getClosestDoor(Vector2f v){
    int r = 30;
    return getClosestDoor(v, r);
}

int Dungeon::getLevelMediaY(){
    return levelmediaY;
}

Door* Dungeon::getClosestDoor(Vector2f v, int r){
    int j = -1;

    for(long unsigned int i=0; i<doors.size(); i++){
        int x = doors[i]->getX() - v.getX();
        int z = doors[i]->getZ() - v.getY();

        x = pow(x, 2);
        z = pow(z, 2);

        int xz = x+z;

        
        int dist = sqrt(xz);

        if(dist <= r){
            j = i;
        }
    }

    if(j == -1){
        return nullptr;
    }else{
        return doors[j];
    }
}

Villager* Dungeon::getClosestNSeal(Vector2f v){

    int r = 60;
    int j = -1;

    for(long unsigned int i=0; i<seals.size(); i++){
        
        if(seals[i]->getType() == S_NORMAL_ENTITY){
            int x = seals[i]->getX() - v.getX();
            int z = seals[i]->getZ() - v.getY();

            x = pow(x, 2);
            z = pow(z, 2);

             int xz = x+z;

            int dist = sqrt(xz);

            if(dist <= r ){
                j = i;
            }
        }
        
    }

    if(j == -1){
        return nullptr;
    }else{
        return seals[j];
    }

}

GrafoP* Dungeon::getGrafo(){
    return dungeonGraph;
}

void Dungeon::setGrafo(GrafoP graph){
    if(dungeonGraph){
        delete dungeonGraph;
        dungeonGraph = nullptr;
    }
    dungeonGraph = new GrafoP(graph);
    //dungeonGraph->drawGraph();
}

Room* Dungeon::getRoomWithMoreAlert(){

    float aux = 0;
    int index = 0;

    for(long unsigned int i=0; i<rooms.size(); i++){
        if(dungeonGraph->nodeInGraph(Vector2f(rooms[i]->getCenterX(), rooms[i]->getCenterZ()))){
            float alert = rooms[i]->getAlert();
            if(alert > aux){
                aux = alert;
                index = i;
            }
        }
    }

    return rooms[index];
}

int Dungeon::getRoomIDPerCenterInt(Vector2f c){
    for(long unsigned int i=0; i<rooms.size(); i++){
        int cx = (int)rooms[i]->getCenterX();
        int cz = (int)rooms[i]->getCenterZ();
        if(cx == c.getX() && cz == c.getY()){
            return rooms[i]->getID();
        }
    }

    return -1;
}

Door* Dungeon::getDoorPerRoomsID(int r1, int r2){
    for(long unsigned int i=0; i<doors.size(); i++){
        int id1 = doors[i]->getID1();
        int id2 = doors[i]->getID2();
        if((id1 == r1 && id2 == r2) || (id1 == r2 && id2 == r1)){
            return doors[i];
        }
    }
    return nullptr;
}

Villager* Dungeon::getClosestQSeal(Vector2f v){

    int r = 60;
    int j = -1;

    for(long unsigned int i=0; i<seals.size(); i++){
        if(seals[i]->getType() == S_QUEST_ENTITY || seals[i]->getType() == QUEEN_ENTITY){
            int x = seals[i]->getX() - v.getX();
            int z = seals[i]->getZ() - v.getY();

            x = pow(x, 2);
            z = pow(z, 2);

            int xz = x+z;

            int dist = sqrt(xz);
            if(dist <= r ){
                j = i;
            }
        }
    }

    if(j == -1){
        return nullptr;
    }else{
        return seals[j];
    }

}

Enemy* Dungeon::getEnemyPerID(int id){
    for(long unsigned int i=0; i<enemies.size(); i++){
        if(enemies[i]->getID() == id){
            return enemies[i];
        }
    }
    return nullptr;
}

Vector3f Dungeon::getClosestPinguPos(int id){
    Enemy* npc1 = getEnemyPerID(id);
    Vector3f pos(0.f,0.f,0.f);
    for(long unsigned int i=0; i<enemies.size(); i++){
        if(enemies[i]->getID() != id){
            float x = enemies[i]->getX();
            float z = enemies[i]->getZ();
            float distx = npc1->getX() - x;
            float distz = npc1->getZ() - z;
            distx = distx * distx;
            distz = distz * distz;
            float dist = sqrt(distx + distz);
            if(dist < 20){
                pos.setX(x);
                pos.setZ(z);
            }
        }
    }
    return pos;
}

Vector3f Dungeon::getNearestObst(float x, float z){

    //CREO EL VECTOR POS DEL OBSTACULO MAS CERCANO
    Vector3f pos(0.f,0.f,0.f);

    int rid = wichRoomIAmAt(x, z);
    bool updatedR = false;

    for(long unsigned int i=0; i<rooms.size() && !updatedR; i++){
        if(rooms[i]->getID() == rid){
            pos = rooms[i]->getClosestDecoration(x,z);
            updatedR = true;
        }
    }

    //FOR PARA LAS CAJAS
    for(long unsigned int i=0; i< dragboxes.size(); i++){
            float x2 = dragboxes[i]->getX();
            float z2 = dragboxes[i]->getZ();
            float distx = x - x2;
            float distz = z - z2;
            distx = distx * distx;
            distz = distz * distz;
            float dist = sqrt(distx + distz);
            if(dist <= 50){
                if(pos.getX() == 0 && pos.getZ() == 0){
                    pos.setX(x2);
                    pos.setZ(z2);
                }else if(pos.getX() > x2 && pos.getZ() > z2){
                    pos.setX(x2);
                    pos.setZ(z2);
                }
            }
    }    
    return pos;
}

std::vector<int> Dungeon::getEnemiesPerRoomID(int idr){
    
    std::vector<int> enemies2;

    for(long unsigned int i=0; i<enemies.size(); i++){
        if(enemies[i]->getSalaActual() == idr){
            if(enemies[i]->isDeath() == false){
                enemies2.push_back(enemies[i]->getID());
            }
        }
    }

    return enemies2;
}

Room* Dungeon::getRoomPerID(int id){
    for(long unsigned int i=0; i<rooms.size(); i++){
        if(rooms[i]->getID() == id){
            return rooms[i];
        }
    }
    return nullptr;
}

Door* Dungeon::getDoorPerID(int id){
    for(long unsigned int i=0; i<doors.size(); i++){
        if(doors[i]->getID() == id){
            return doors[i];
        }
    }
    return nullptr;
}

Door* Dungeon::getDoorPerIndex(long unsigned int i){
    if(i < doors.size()){
        return doors[i];
    }else{
        return nullptr;
    }
}

DragBox* Dungeon::getDragBoxPerID(int _id){
    
    DragBox* db = nullptr;

    for(long unsigned int i=0; i<dragboxes.size(); i++){
        if(dragboxes[i]->getID() == _id){
            db = dragboxes[i];
            break;
        }
    }

    return db;
}

void Dungeon::clearState(){
    
    for(long unsigned int i=0; i<enemies.size(); i++){
        if(enemies[i] != nullptr){
            delete enemies[i];
            enemies[i] = nullptr;
        }
    }
    enemies.resize(0);

    for(long unsigned int i=0; i<rooms.size(); i++){
        if(rooms[i] != nullptr){
            delete rooms[i];
            rooms[i] = nullptr;
        }
    }
    rooms.resize(0);

    for(long unsigned int i=0; i<doors.size(); i++){
        if(doors[i] != nullptr){
            delete doors[i];
            doors[i] = nullptr;
        }
    }
    doors.resize(0);

    for(long unsigned int i=0; i<seals.size(); i++){
        if(seals[i] != nullptr){
            delete seals[i];
            seals[i] = nullptr;
        }
    }
    seals.resize(0);

    if(dungeonGraph){
        delete dungeonGraph;
        dungeonGraph = nullptr;
    }

    dragboxes.clear();
}

int Dungeon::wichRoomIAmAt(float x, float z){
    for(long unsigned int i=0; i<rooms.size(); i++){
        if(rooms[i]->getMinX() < x && rooms[i]->getMaxX() > x){
            if(rooms[i]->getMinZ() < z && rooms[i]->getMaxZ() > z){
                return rooms[i]->getID();
            }
        }
    }

    return -1;
}

float Dungeon::calculeDistance(Vector2f a, Vector2f b){
    float x = a.getX();
    float z = a.getY();
    float dx = b.getX() - x;
    float dz = b.getY() - z;
    dx = dx*dx;
    dz = dz*dz;
    return sqrt(dx + dz);
}

Dungeon::~Dungeon()
{
    //dtor
}

void Dungeon::restart(){

    auto* pl = Player::Instance();

    clearState();
    loaded = false;
    loadsave = true;
    loadlevel = true;
    //pl->setPosition(Vector3f(75.0f, 5.0f, 0.0f));
    //pl->setLifes(GLOBAL_MAX_LIFE_PLAYER);
    PhysicsEngine::Instance()->clearKinematic();
    Motor3D::Instance()->insertLifes(pl->getLifes());

    pl = nullptr;
}

void Dungeon::setGameOver(bool g){
    gameOver = g;
}

void Dungeon::setResetPlayer(){
    resetPlayer = true;
}

void Dungeon::setBossAnimation(bool tf){
    transition = true;
    animation_clock = 0;
    init_boss_animation = tf;
}

void Dungeon::setLevelMediaY(int media){
    levelmediaY = media;
}

void Dungeon::startBossBattle(){
    /*auto* d = Dialog_State::Instance();
    d->setParams(1, 0, P_KPONGO_ENTITY);
    Game::Instance()->setState(Estado::StateType::DIALOG_STATE);*/
    m3D::Motor3D::Instance()->insertLifes(Player::Instance()->getLifes());
    init_boss_animation = false;
    bool done = false;
    for(long unsigned int i=0; i<doors.size() && !done; i++){
        if(doors[i]->getType() == BOSS_DOOR_ENTITY){
            doors[i]->setClosed(true);
            doors[i]->setLocked(true);
            done = true;
        }
    }
}

void Dungeon::setLoaded(bool l){
    loaded = l;
}

void Dungeon::setLOD(Decoration* d, float r){
	auto* pl = Player::Instance();
	float dist = calculeDistance(Vector2f(d->getX(),d->getZ()), Vector2f(pl->getX(),pl->getZ()));

    if(dist < r){
        m3D::Motor3D::Instance()->setLODdeco(d->getNode(), false);
    }else{
        m3D::Motor3D::Instance()->setLODdeco(d->getNode(), true);
    }
}

void Dungeon::setAnimations(int s1,int s2){
    for(long unsigned int i=0; i < rooms.size(); i++){
        if(s1 != -1 && rooms[i]->getID() == s1){
            rooms[i]->setTorchsActive(true);
        }
        else if(s2 != -1 && rooms[i]->getID() == s2){
            rooms[i]->setTorchsActive(false);
        }
    }
}

void Dungeon::setKPongoBattle(){
    for(long unsigned int i=0; i<enemies.size(); i++){
        if(enemies[i]->getType() == P_KPONGO_ENTITY){
            P_KPongo* pk = dynamic_cast<P_KPongo*> (enemies[i]);
            AudioManager::Instance()->stop("Musica/Juego/Castillo2");
            pk->startBattle();
            pk->setY(3);
            pk->setInitY(21);
        }    
    }
}

void Dungeon::setLevel(int lvl){
    lastlevel = level;
    level = lvl;
}

int Dungeon::getLevel(){
    return level;
}

void Dungeon::changeLevel(){

    auto* m = Motor3D::Instance();
    m->setLoadingInactive(false);
    auto* fn = FactoriaNivel::Instance();
    m->drawLoadingScreen();
    fn->saveGame(lastlevel);
    m->drawLoadingScreen();
    clearState();
    setLoaded(false);
    loadlevel = true;
    m->drawLoadingScreen();
    PhysicsEngine::Instance()->clearKinematic();
    m->drawLoadingScreen();
    transition = true;
    tnumber = 2;
}

void Dungeon::setLoadSaveData(bool s){
    loadsave = s;
}

void Dungeon::drawDebug(){

    if(debug == false){
        debug = true;
        /*
        auto* m = m3D::Motor3D::Instance();
        
        for(int i=0; i<enemies.size();i++){

            if(enemies[i]->isDeath() == false){
                m->drawCylinder(enemies[i]->getDebugNodeH(), enemies[i]->getHear(), Vector3f(enemies[i]->getX(), enemies[i]->getY(), enemies[i]->getZ()),'r');
            }
        }


        m = nullptr;*/

    }else{
        debug = false;
    }
}

void Dungeon::setChangeTextures(bool f){
    changeTextures = f;
}


int Dungeon::getFPS(){
    return fps;
}

