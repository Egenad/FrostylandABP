#include "Player.h"
#include "EventManager.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include "Game.h"
#include "Sword.h"
#include "Slingshot.h"
#include "Hammer.h"
#include "PhysicsEngine.h"
#include "AudioManager.h"

/////////////////////////////////////////
// INSTANCE
/////////////////////////////////////////

Player* Player::Instance(){
    static Player pinstance;
    return &pinstance;
}


/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Player::Player(){}

Player::~Player()
{
    //dtor
}

void Player::initPlayer(){
    ps_x = 0.f;
    ps_y = 7.f;
    ps_z = 0.f;
    //r = 15.f;
    width = 30;
    height = 40;
    angle = 0;
    rleft = true;
    velocidad = 80.0;
    vida = GLOBAL_MAX_LIFE_PLAYER;
    dinero = 0;
    next_attack = 0.6;
    attacking = false;
    salaActual = -1;
    lastSala = -1;
    pushed = false;
    pushed_left = false;
    pushed_down = false;
    push_time = 0;
    interactTime = 0;

    //inventory
    for(int i=0;i<GLOBAL_FOOD_SIZE;i++){
        food[i]=-1;
    }
    for(int i=0;i<GLOBAL_WEAPONS_SIZE;i++){
        weapons[i]=false;
    }

    e_weapon = nullptr;

    vision_vector = new Vector3f(0.f,0.f,1.f);

    boss_key = false;
    jail_key = false;
    level_key = false;
    keys=0;

    pl_camera = new Camera(ps_z);
    
    walkDirection = btVector3(0.0, 0.0, 0.0);
    walkSpeed=0.0f;

    ismoving = true;
    m_down = true;
    m_up = true;
    m_left = true;
    m_right = true;
    dragBox = nullptr;

    //--------Animation------------
    animationtime = 0.0;
    animationframe = 1;
    resetanimation = true;
}

void Player::clearInstance(){
    if(vision_vector){
        delete vision_vector;
        vision_vector = nullptr;
    }
    if(e_weapon){
        delete e_weapon;
        e_weapon = nullptr;
    }
    if(pl_camera){
        delete pl_camera;
        pl_camera = nullptr;
    }
    m3D::Motor3D::Instance()->clearNode(player);
    m3D::Motor3D::Instance()->clearNode(shadow);
}


/////////////////////////////////////////
// MAIN METHODS
/////////////////////////////////////////

void Player::createPlayerKinematic(){
    PhysicsEngine::Instance()->createPlayer(ps_x,ps_y,ps_z);
}

void Player::updateEntity(){

    auto* d = Dungeon::Instance();
    auto* m = m3D::Motor3D::Instance();

    if(pushed){
        if(push_time >= 0.5){
            pushed = false;
            push_time = 0;
        }else{
            push_time += m->getDeltaTime();

            float aux = walkSpeed;
            walkSpeed = 3;

            if(pushed_left){
                PhysicsEngine::Instance()->movePlayerLeft();
                setVisionVectorValues(-1,0,0);
                ismoving=true;
            }else{
                PhysicsEngine::Instance()->movePlayerRight();
                setVisionVectorValues(1,0,0);
                ismoving=true;
            }

            if(pushed_down){
                PhysicsEngine::Instance()->movePlayerBackward();
                setVisionVectorValues(0,0,-1);
                ismoving=true;
            }else{
                PhysicsEngine::Instance()->movePlayerForward();
                setVisionVectorValues(0,0,1);
                ismoving=true;
            }

            walkSpeed = aux;
        }
    }

    PhysicsEngine::Instance()->updatePlayerPosition(); //updateamos como le afectan las fisicas a player

    if(rleft && angle != 0 && dragBox==nullptr){
        angle -= 20;
        m->rotateEntity(player, -20);
    }else if(!rleft && angle != 180 && dragBox==nullptr){
        angle += 20;
        m->rotateEntity(player, 20);
    }

    int aux = d->wichRoomIAmAt(ps_x, ps_z);

    if(dragBox == nullptr){
        setVelocity(80.0f);
        if(attacking == true){
            if(next_attack > 0.3){
                attacking = false;
                resetanimation = true;
            }else{
                next_attack += m->getDeltaTime();
            }
        }
    }else{
        setVelocity(20.0f);
        dragBox->updateEntity();
        int aux2 = d->wichRoomIAmAt(dragBox->getX(), dragBox->getZ());
        if(aux!=aux2){
            DragBox* box = dynamic_cast<DragBox*> (dragBox);
            box->moveDragBox();
        }
    }

    if(pl_camera){ 
        pl_camera->updateCamera(ps_x, ps_y, ps_z);
    }
    
    ismoving = false;

    if(aux != salaActual && aux != -1){
        lastSala = salaActual;
        salaActual = aux;
        d->setAnimations(salaActual, lastSala);
        int auxc = pl_camera->getCameraState();
        if(auxc == 1 || auxc == 2){
            pl_camera->setCameraState(3);
        }
    }

    interactTime += m->getDeltaTime();

    //Dibujar el boton que debe pulsar y en controles cambia la imagen
    checkInteract();

    d = nullptr;
    m = nullptr;
}

void Player::updateCamera(){
    if(pl_camera) pl_camera->updateCamera(ps_x, ps_y, ps_z);
}

void Player::setPCameraState(int state){
    if(pl_camera) pl_camera->setCameraState(state);
}

void Player::resetCameraPosition(){
    if(pl_camera) pl_camera->setCameraPosition(ps_x, ps_y, ps_z);
}

void Player::checkInteract(){
    
    if(salaActual != -1){

        auto* d = Dungeon::Instance();
        Room* r = d->getRoomPerID(salaActual);
        bool foundInteract = false;

        if(r){
            auto* m = m3D::Motor3D::Instance();
            //Chest and DragBox
            if(r->interactibleInRange(ps_x,ps_z)){
                foundInteract = true; 
            }
            //Doors
            if(!foundInteract){
                Door* door = d->getClosestDoor(Vector2f(ps_x, ps_z));
                if(door){
                    if(door->getClosed() && !door->getLocked()){
                        m->drawInfoControl(2);
                        foundInteract = true;
                    }
                }
            }
            //Dialog (Quest)
            if(!foundInteract){

                Villager* v = d->getClosestQSeal(Vector2f(ps_x, ps_z));

                Seal_Quest* seal = dynamic_cast<Seal_Quest*> (v);
                if(seal){
                    m->drawInfoControl(3);
                    foundInteract = true;
                }else{
                    Queen* q = dynamic_cast<Queen*> (v);
                    if(q){
                        m->drawInfoControl(3);
                        foundInteract = true;
                    }
                }
            }
    
            
            if(foundInteract){
                m->drawInfoInteract(true);
            }else{
                m->drawInfoControl(-1);
                m->drawInfoInteract(false);
            }
            m = nullptr;
        
        }

        r=nullptr;
        d=nullptr;

    }

}

void Player::interactObject(){
    if(interactTime > 0.5 && !attacking){
        if(dragBox == nullptr){
            auto* d = Dungeon::Instance();
            int idr = d->wichRoomIAmAt(ps_x, ps_z);
            if(idr!=-1){
                Room* r = d->getRoomPerID(idr);
                bool interacted = false;
                if(r){
                    if(r->interactibleHere()){ //check if the room we are at has any interactibles placed inside
                        interacted = r->interactueWithObjectInRange(ps_x, ps_z);
                        if(interacted) interactTime = 0;
                    }
                
                    if(!interacted){
                        //If we didnt interact with an object, we check if we can interact with a door
                        //We want to try it ONLY with the doors that has the ID of the room in which the player is in it
                        Door* door = d->getClosestDoor(Vector2f(ps_x, ps_z));
                        if(door){
                            door->interactPlayer(ps_x, ps_z);
                            interactTime = 0;
                            interacted = true;
                        }
                    }
                    if(!interacted){
                        //If we didnt interact with an object nor a Door, we check if we can interact with a NPC
                        Villager* v = d->getClosestQSeal(Vector2f(ps_x, ps_z));
                        Seal_Quest* seal = dynamic_cast<Seal_Quest*> (v);
                        if(seal){
                            if(seal->getFinished() == false){
                                if(seal->getState() == 1){
                                    seal->endQuest();
                                }
                                seal->setState(1);
                            }
                            pl_camera->setCameraState(4);
                            seal->callDialog(seal->getDialog());
                            interactTime = 0;
                            interacted = true;
                        }else{
                            Queen* queen = dynamic_cast<Queen*> (v);
                            if(queen){
                                pl_camera->setCameraState(4);
                                queen->callDialog(queen->getDialog());
                                interactTime = 0;
                                interacted = true;
                            }
                        }
                    }
                }
                r = nullptr;
            }
            d = nullptr;
        }else{
            m_right = true;
            m_left = true;
            m_down = true;
            m_up = true;
            dragBox = nullptr;
        }
    }
}

void Player::attack(){

    auto* m = m3D::Motor3D::Instance();

    if(e_weapon != nullptr && next_attack > 0.3 && dragBox == nullptr){

        next_attack = 0;

        //First we will check if there are enemies at the room the player is standing
        auto* d = Dungeon::Instance();

        int idr = d->wichRoomIAmAt(ps_x, ps_z);
        if(idr != -1){
            std::vector<int> enemies = d->getEnemiesPerRoomID(idr);

            //Animation
            if(attacking == false){
                attacking = true;
                e_weapon->playAudio(0);
                m->setMeshTexture(player,"../assets/textures/eika_a.png");
            }

            if(enemies.size() > 0){

                for(long unsigned int i=0; i<enemies.size(); i++){

                    //check distance if they are near
                    float dist = calculateDistanceEnemy(enemies[i]);
                    bool hit = false;

                    Enemy* npc = Dungeon::Instance()->getEnemyPerID(enemies[i]);

                    if(npc){

                        if(dist <= e_weapon->getHitRange()){            
                            hit = true;
                        }

                        if(hit){
                            //Atack enemy
                            if(npc->isDeath() == false){
                                npc->reduceLife(e_weapon->getDamage());
                                e_weapon->playAudio(1);
                                playAudio(0);
                            }
                        }
                        npc = nullptr;
                    }
                }
            }
        }
        d = nullptr;
    }else{
        next_attack += m->getDeltaTime();
    }
    m = nullptr;
}

void Player::moveUp(){
    //ps_z = ps_z + velocidad * m3D::Motor3D::Instance()->getDeltaTime();

    if(m_up && !attacking && !pushed){
        PhysicsEngine::Instance()->movePlayerForward();
        setVisionVectorValues(0,0,1);
        ismoving=true;
    }
}

void Player::moveDown(){
    //ps_z = ps_z - velocidad * m3D::Motor3D::Instance()->getDeltaTime();

    if(m_down && !attacking && !pushed){
        PhysicsEngine::Instance()->movePlayerBackward();
        setVisionVectorValues(0,0,-1);
        ismoving=true;
    }
}

void Player::moveLeft(){
    //ps_x = ps_x - velocidad * m3D::Motor3D::Instance()->getDeltaTime();

    rleft = true;

    if(m_left && !attacking && !pushed){
        PhysicsEngine::Instance()->movePlayerLeft();
        setVisionVectorValues(-1,0,0);
        ismoving=true;
    }
}

void Player::moveRight(){
    //ps_x = ps_x + velocidad * m3D::Motor3D::Instance()->getDeltaTime();

    rleft = false;

    if(m_right && !attacking){
        PhysicsEngine::Instance()->movePlayerRight();
        setVisionVectorValues(1,0,0);
        ismoving=true;
    }
}

void Player::receiveDamage(int damage){
    auto* m = m3D::Motor3D::Instance();
    auto* e = EventManager::Instance();
    auto* a = AudioManager::Instance();

    playAudio(1);
    vida -= damage;
    m->insertLifes(vida);

    if(vida <= 4){
        if(a->isPlaying("Diseño/Juego/VidaBaja") == false){
            a->shootAudio("Diseño/Juego/VidaBaja", -1, 0);
        }
    }

    if(vida <= 0){ //Termina la partida --> Game Over
        playAudio(2);
        a->stop("Diseño/Juego/VidaBaja");
        if(a->isPlaying("Musica/Juego/Castillo2")){
            a->stop("Musica/Juego/Castillo2");
        }else if(a->isPlaying("Musica/Juego/Batalla")){
            a->stop("Musica/Juego/Batalla");
        }
        e->addEvent(Event{Type::GAME_OVER_CHANGE});
    }

    m = nullptr;
    e = nullptr;
    a = nullptr;
}

void Player::addMoney(int plus){
    AudioManager::Instance()->shootAudio("Foleys/Objetos/Moneda", -1, 0);
    dinero += plus;
}

void Player::restMoney(int minus){
    AudioManager::Instance()->shootAudio("Foleys/Objetos/Comprar", -1, 0);
    dinero -= minus;
}

void Player::addKey(){
    if(keys<GLOBAL_MAX_KEYS){
        keys += 1;
        AudioManager::Instance()->shootAudio("Foleys/Objetos/Llave", -1, 0);
    }
}

void Player::restKey(){
    if(keys>0)
        keys -= 1;
}

float Player::calculateDistanceEnemy(int id){

    Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);

    //Check distance between npc and player
    float x = npc->getX() - ps_x;
    x = pow(x,2);
    float z = npc->getZ() - ps_z;
    z = pow (z,2);
    float xz = x+z;

    float distance = sqrt(xz);

    npc = nullptr;

    return distance;
}

float Player::calculateAngleEnemy(int id){

    Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);

    //calculate vector director PlayerEnemy
    int __x = ps_x - npc->getX();
    int __y = ps_y - npc->getY();
    int __z = ps_z - npc->getZ();
    float vvx = npc-> getVisionVectorX();
    float vvy = npc-> getVisionVectorY();
    float vvz = npc-> getVisionVectorZ();

    //Calculate angle
    float dp = (vvx*__x) + (vvy*__y) + (vvz*__z);
    float modd = sqrt(pow(vvx, 2) + pow(vvy, 2) + pow(vvz, 2));
    float modp = sqrt(pow(__x, 2) + pow(__y, 2) + pow(__z, 2));
    float moddp = modd*modp;
    float cos_ang = 0.1;

    if(moddp != 0){
        cos_ang = dp/moddp;
    }

    float ang = acos(cos_ang);

    npc = nullptr;

    return ang;
}

void Player::playAudio(int a){
    auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.3){
        r = 0.4;
    }else if(r == 0.5){
        r = 0.4;
    }else if(r == 0.6){
        r = 0.55;
    }

    //std::cout<< "random -> " << r << "\n";

    if(a==0){
        //Attack
        am->shoot3DAudio("Voces/Player/Atacar",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==1){
        //RDamage
        am->shoot3DAudio("Voces/Player/RDaño",0,r, Vector3f(ps_x, ps_y, ps_z));
    }else if(a==2){
        //Die
        am->shoot3DAudio("Voces/Player/Morir",0,r, Vector3f(ps_x, ps_y, ps_z));

    }

    am = nullptr;
}

/////////////////////////////////////////
// GETTERS
/////////////////////////////////////////

int Player::getSalaActual(){
    return salaActual;
}

int Player::getLastSala(){
    return lastSala;
}

m3D::Node& Player::getNode(){
    return player;
}

m3D::Node& Player::getShadow(){
    return shadow;
}

Vector2f Player::getSize(){
    return Vector2f(width,height);
}

int Player::getLifes(){
    return vida;
}

int Player::getMoney(){
    return dinero;
}

int Player::getKeys(){
    return keys;
}

bool Player::getBossKey(){
    return boss_key;
}

bool Player::getLevelKey(){
    return level_key;
}

bool Player::getJailKey(){
    return jail_key;
}

int* Player::getFood(){
    return food;
}

bool* Player::getWeapons(){
    return weapons;
}

float Player::getVelocity(){
    return velocidad;
}

bool Player::getMoving(){
    return ismoving;
}

bool Player::allAnglesofMovement(){
    if(m_right && m_left && m_down && m_up){
        return true;
    }
    return false;
}

float Player::getVisionVectorX(){
    return vision_vector->getX();
}
float Player::getVisionVectorY(){
    return vision_vector->getY();
}
float Player::getVisionVectorZ(){
    return vision_vector->getZ();
}
int Player::getFoodType(int var){
    return food[var];
}

Wearable* Player::getWearable(){
    return e_weapon;
}

btVector3& Player::getWalkDirection(){
    return walkDirection;
}

btScalar& Player::getWalkSpeed(){
    return walkSpeed;
}

DragBox* Player::getInteractible(){
    return dragBox;
}

bool Player::getBlockMovLeft(){
    return m_left;
}

bool Player::getBlockMovRight(){
    return m_right;
}

bool Player::getBlockMovUp(){
    return m_up;
}

bool Player::getBlockMovDown(){
    return m_down;
}

Camera* Player::getCamera(){
    return pl_camera;
}

/////////////////////////////////////////
// SETTERS
/////////////////////////////////////////

void Player::setPushed(bool _p){
    pushed = _p;
}

void Player::setPushedVelocity(bool _down, bool _left){
    pushed_down = _down;
    pushed_left = _left;
}

void Player::setWalkSpeed(btScalar sca){
    walkSpeed=sca;
}

void Player::setWalkDirection(btVector3 vec){
    walkDirection=vec;
}

void Player::setCoins(int coins){
    int aux = rand() % coins;
    aux += coins;
    dinero += aux;
}

void Player::setMoney(int m){
    dinero = m;
}

void Player::setVelocity(float v){
    velocidad=v;
}

void Player::setKeys(int k){
    keys = k;
}

void Player::setBossKey(bool b){
    boss_key = b;
}

void Player::setLevelKey(bool b){
    level_key = b;
}

void Player::setJailKey(bool b){
    jail_key = b;
}

void Player::setDraggeable(DragBox* idr){
    dragBox = idr;
    m3D::Motor3D::Instance()->setMeshTexture(player, "../assets/textures/eika2_e.png" );
}

void Player::heal(int x){
    auto* a = AudioManager::Instance();

    if(vida+x<=GLOBAL_MAX_LIFE_PLAYER){
        vida = vida + x;
    }else{
        vida = GLOBAL_MAX_LIFE_PLAYER;
    }
    //sound
    m3D::Motor3D::Instance()->insertLifes(vida);

    a->shootAudio("Foleys/Objetos/Comer", -1,0);
    if(vida > 4){
        if(AudioManager::Instance()->isPlaying("Diseño/Juego/VidaBaja") == true){
            a->stop("Diseño/Juego/VidaBaja");
        }
    }

    a = nullptr;
}

void Player::setLifes(int x){
    vida = x;
}

void Player::eraseFood(int i){
    food[i]=-1;
    //reorder the food array
    for(int j=i;j<GLOBAL_FOOD_SIZE;j++){
        if(food[j+1]!=-1&&j<GLOBAL_FOOD_SIZE-1){
            int aux;
            aux=food[j];
            food[j]=food[j+1];
            food[j+1]=aux;
        }else{
            break;
        }
    }
    
}

void Player::setMRight(bool v){
    m_right=v;
}

void Player::setMLeft(bool v){
    m_left=v;
}

void Player::setMUp(bool v){
    m_up=v;
}

void Player::setMDown(bool v){
    m_down=v;
}

void Player::equipWeapon(int i){
    int a = -1;
    if(e_weapon==nullptr){
        Wearable* w=nullptr;
        switch(i){
            case 0:
                w = new Sword();
                break;
            case 1:
                w = new Slingshot();
                break;
            case 2:
                w = new Hammer();
                break;
        }
        a = i;
        e_weapon = w;
        w = nullptr;   
    }else{
        delete e_weapon;
        e_weapon = nullptr;
    }

    Motor3D::Instance()->drawEquipedWeapon(a);
}

void Player::setIsMoving(bool f){
    ismoving = f;
}

void Player::setPosition(Vector3f pos){

    auto* d = Dungeon::Instance();
    auto* m = Motor3D::Instance();

    std::string plus = "";

    if(dragBox != nullptr){
        plus = "_e";
    }

    if(!attacking){
        int sl = d->wichRoomIAmAt(ps_x, ps_z);
        if(sl != -1){
            Room* r = d->getRoomPerID(sl);
            if(ismoving || r->getType() == HOLLOW_ROOM){
                ps_x = pos.getX();
                ps_y = pos.getY();
                ps_z = pos.getZ();
                m->setPositionEntity(player,ps_x,ps_y,ps_z);
                m->setPositionEntity(shadow,ps_x,ps_y,ps_z);
                if(ismoving)updateAnimation();
            }else if(resetanimation == true){
                resetanimation = false;
                m->setMeshTexture(player,"../assets/textures/eika"+plus+".png");
            }
        }else{
            if(ismoving){
                ps_x = pos.getX();
                ps_y = pos.getY();
                ps_z = pos.getZ();
                m->setPositionEntity(player,ps_x,ps_y,ps_z);
                m->setPositionEntity(shadow,ps_x,ps_y,ps_z);
                updateAnimation();
            }else if(resetanimation == true){
                resetanimation = false;
                m->setMeshTexture(player,"../assets/textures/eika"+plus+".png");
            }
        }
        if(pos.getX() == 2500 && pos.getY() == 2500 && pos.getZ() == 2500){
            ps_x = pos.getX();
            ps_z = pos.getZ();
            m->setPositionEntity(player,ps_x,ps_y,ps_z);
            m->setPositionEntity(shadow,ps_x,ps_y,ps_z);
        }
    }
}

void Player::setPositionEntity(Vector3f pos){
    auto* m = m3D::Motor3D::Instance();
    ps_x = pos.getX();
    ps_y = pos.getY();
    ps_z = pos.getZ();
    m->setPositionEntity(player, ps_x, ps_y, ps_z);
    m->setPositionEntity(shadow,ps_x,ps_y,ps_z);
}

void Player::setPositionPhysic(){
    PhysicsEngine::Instance()->setPlayerPosition(ps_x,ps_y,ps_z);
}

void Player::addFood(int tipo){
    for(int i=0;i<GLOBAL_FOOD_SIZE;i++){
        if(food[i] == -1){
            food[i] = tipo;
            break;
        }
    }
}

void Player::addWeapon(int type){
    switch(type){

        case 0:
            weapons[0]=true;
        break;

        case 1:
            weapons[1]=true;
        break;

        case 2:
            weapons[2]=true;
        break;

    }  
   
}

void Player::setVisionVectorValues(float x, float y, float z){
    vision_vector->changeAllValues(x,y,z);
}

void Player::setCState(int s){
    pl_camera->setCameraState(s);
}

void Player::fullInventory(){

    for(int i=0;i<GLOBAL_FOOD_SIZE;i++){
        if(i < GLOBAL_FOOD_SIZE/4){
            food[i] = 0;
        }else if(i < 2*GLOBAL_FOOD_SIZE/4){
            food[i]= 1;
        }else if(i< 3*GLOBAL_FOOD_SIZE/4){
            food[i] = 2;
        }else{
            food[i] = 3;
        }
    }
    for(int i=0;i<GLOBAL_WEAPONS_SIZE;i++){
        weapons[i]=true;
    }

    boss_key = true;
    jail_key = true;
    level_key = true;
    keys = 2;
}

void Player::setInteractTime(float t){
    interactTime = t;
}

void Player::updateAnimation(){

    auto* m = Motor3D::Instance();

    std::string plus = "";

    if(dragBox != nullptr){
        plus = "_e";
    }

    if(animationtime > 0.15){
        if(animationframe == 1 || animationframe == 3){
            m->setMeshTexture(player,"../assets/textures/eika"+ plus +".png");
        }else if(animationframe == 2){
            m->setMeshTexture(player,"../assets/textures/eika2"+ plus +".png");
        }else if(animationframe == 4){
            m->setMeshTexture(player,"../assets/textures/eika3"+ plus +".png");
        }
        
        animationframe++;

        if(animationframe == 5)
            animationframe = 1;

        animationtime = 0;
        resetanimation = true;
    }else{
        animationtime += m->getDeltaTime();
    }
}