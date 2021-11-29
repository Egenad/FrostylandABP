#include "Motor3D.h"
#include "Player.h"

using namespace m3D;

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Motor3D* Motor3D::Instance(){
    static Motor3D pinstance;
    return &pinstance;
}

Motor3D::Motor3D()
{
    //input_facade = new IrrInputFacade;
    //render_facade = new RenderFacade(input_facade);
    render_facade = new AnimaRender();
}

void Motor3D::clearInstance(){
    delete render_facade;
    render_facade = nullptr;
    //delete input_facade;
    //input_facade = nullptr;
}

void Motor3D::clearDungeonMesh(){
    render_facade->clearDungeonMesh();
}

Motor3D::~Motor3D(){

}


/////////////////////////////////////////
// DRAW METHODS
/////////////////////////////////////////


void Motor3D::drawDecoration(Node& node, float x, float y, float z, int angle, int type){
    if(type == DECORATION_TORCH){
        render_facade->drawTorch(node,x,y,z,angle);
    }else if(type == DECORATION_TABLE){
        render_facade->drawTable(node,x,y,z,angle);
    }else if(type == DECORATION_BARREL){
        render_facade->drawBarrel(node,x,y,z,angle);
    }else if(type == DECORATION_BOX){
        render_facade->drawBox(node,x,y,z,angle);
    }else if(type == DECORATION_BED){
        render_facade->drawBed(node,x,y,z,angle);
    }else if(type == DECORATION_CHAIR){
        render_facade->drawChair(node,x,y,z,angle);
    }else if(type == DECORATION_FLAG){
        render_facade->drawFlag(node,x,y,z,angle);
    }else if(type == DECORATION_CHAIN){
        render_facade->drawChain(node,x,y,z,angle);
    }else if(type == DECORATION_SOLDIER){
        render_facade->drawPSD(node,x,y,z,angle);
    }else if(type == DECORATION_FLAG){
        render_facade->drawFlag(node,x,y,z,angle);
    }else if(type == DECORATION_STATUE1){
        render_facade->drawS1(node,x,y,z,angle);
    }else if(type == DECORATION_STATUE2){
        render_facade->drawS2(node,x,y,z,angle);
    }else if(type == DECORATION_COLUMN){
        render_facade->drawColumn(node,x,y,z,angle);
    }else if(type == DECORATION_THRONE){
        render_facade->drawThrone(node,x,y,z,angle);
    }else if(type == DECORATION_PAINTING){
        render_facade->drawPainting(node,x,y,z,angle);
    }else if(type == DECORATION_SWORDDECO){
        render_facade->drawSwordDeco(node,x,y,z,angle);
    }else if(type == DECORATION_LANCE){
        render_facade->drawLance(node,x,y,z,angle);
    }
}

void Motor3D::drawLoadingScreen(){
    render_facade->drawLoadingScreen();
}

void Motor3D::drawEntity(Node& node, float r, float x, float y, float z, char c){
    render_facade->drawEntity(/*node.getNode(),*/ r, x, y, z, c);
}

void Motor3D::drawDescription(){
    render_facade->RenderInventoryDescription();
}

void Motor3D::loadTextures(int state){
    render_facade->loadTextures(state);
}

void Motor3D::drawDialog(int a, EntityType b){
    render_facade->RenderDialog(a,b);
}

void Motor3D::drawBillBoardEntity(Node& node, Node& shadow, float x, float y, float z, EntityType type){
    render_facade->drawBillBoardEntity(node, shadow, x, y, z, type);
}

void Motor3D::beginScene(bool a, bool b, int c, int d, int e, int f){
    render_facade->beginScene(a,b,c,d,e,f);
}

void Motor3D::drawScene(){
    render_facade->drawScene();
}

void Motor3D::dibujarMenu(int option){
    render_facade->dibujarMenu(option);
}

void Motor3D::dibujarMenuPause(int option){
    render_facade->drawPauseButtons(option);
}

void Motor3D::dibujarMenuGameOver(int option){
    render_facade->drawGameOverMenu(option);
}

void Motor3D::drawDoor(Node& n, float x, float y, float z, bool down, EntityType type){
    render_facade->drawDoor(n,x,y,z,down,type);
}

void Motor3D::drawChest(Node& n, float x, float y, float z){
    render_facade->drawChest(n,x,y,z);
}

void Motor3D::drawPlatform(Node& n, float x, float y, float z){
    render_facade->drawPlatform(n,x,y,z);
}

void Motor3D::drawInteractible(Node& n, float x, float y, float z, EntityType t){
    if(t == PRESSED_INTERACTIBLE){
        render_facade->drawPlate(n,x,y,z);
    }else if(t == DRAGGABLE_INTERACTIBLE){
        render_facade->drawBox(n,x,y,z);
    }
}

void Motor3D::drawLine(Vector3f p1, Vector3f p2, char c){
    render_facade->drawLine(p1.getX(), p1.getY(), p1.getZ(), p2.getX(), p2.getY(), p2.getZ(), c);
}

void Motor3D::drawReceived(int ob){
    render_facade->drawObjectReceived(ob);
}

void Motor3D::drawInventory(int food[],bool weapons[], int keys, bool boss_key, bool level_key, bool jail_key, float x, float y, int money){
    render_facade->drawInventory(food,weapons,keys,boss_key,level_key, jail_key, x, y, money);
}

void Motor3D::drawItemOptions(float x, float y, int selector, int item){
    render_facade->drawItemOptions(x,y,selector,item);
}

void Motor3D::drawDatosEnemy(int id){
    render_facade->RenderDatosEnemy(id);
}

void Motor3D::drawDatosPlayer(){
    render_facade->RenderDatosPlayer();
}

void Motor3D::drawInfoInteract(bool a){
    render_facade->drawInfoInteract(a);
}

void Motor3D::drawInfoControl(int a){
    render_facade->drawInfoControl(a);
}

void Motor3D::drawEquipedWeapon(int w){
    render_facade->drawEquipedWeapon(w);
}
/////////////////////////////////////////
// GETTERS
/////////////////////////////////////////

float Motor3D::getDeltaTime(){
    return render_facade->getDeltaTime();
}

float Motor3D::getNow(){
    return render_facade->getNow();
}


/////////////////////////////////////////
// MAIN METHODS
/////////////////////////////////////////

void Motor3D::redibujarOptions(int a,int b){
    render_facade->redibujarOptions(a,b);
}

void Motor3D::redibujarOptions(int a){
    render_facade->redibujarOptions(a);
}

void Motor3D::setLODdeco(Node& n, bool sn){
    render_facade->setLODdecoration(n, sn);
}

void Motor3D::setPositionEntity(Node& n, float x, float y, float z){
    render_facade->setPositionEntity(n,x,y,z);
}

void Motor3D::setLoadingInactive(bool f){
    render_facade->setLoadingInactive(f);
}

void Motor3D::rotateEntity(Node& n, float ang){
    render_facade->rotateEntity(n,ang);
}

void Motor3D::updatePlCamera(Vector3f position, Vector3f target){
    render_facade->updatePlCamera(position.getX(), position.getY(), position.getZ(), target.getX(), target.getY(), target.getZ());
}

void Motor3D::inicializarDungeon(int lvl){
    render_facade->inicializarDungeon(lvl);
}

void Motor3D::setWindowSize(float w,float h){
    render_facade->setWindowSize(w,h);
}

int Motor3D::sigueActivo(){
    return render_facade->sigueActivo();
}

void Motor3D::insertLifes(int lifes){
    render_facade->insertLifes(lifes);
}

bool Motor3D::onKeyPressed(char key){
    return render_facade->isKeyDown(key);
}

bool Motor3D::isconectedJoystick(){
    bool conected = false;
    if(render_facade->isJoystickPresent() == 1){
        conected = true;
    }
    return conected;
}

void Motor3D::checkJoystickAxes(float& x, float& y){
    render_facade->checkJoystickAxes(x,y);
}

void Motor3D::initController(){
    render_facade->initJoystick();
}

bool Motor3D::onButtonPressed(char button){
    return render_facade->onButtonPressed(button);
}

void Motor3D::clear(){
    render_facade->clear();
}

void Motor3D::cleanDialogText(){
    render_facade->cleanDialogText();
}

void Motor3D::clearScene(){
    render_facade->clearScene();
}

void Motor3D::activarBillboards(Estado::StateType t){
    render_facade->activateBillboards(t);
}

void Motor3D::setDebugDrawer(ADebug* debug){
    render_facade->setDebugDrawer(debug);
}

void Motor3D::setRadius(float r){
    render_facade->setRadius(r);
}

void Motor3D::setTransition(bool f){
    render_facade->setTransition(f);
}

void Motor3D::playAnimation(Node& n){
    render_facade->playAnimation(n);
}

void Motor3D::createParticleSystem(std::string shape, float px, float py, float pz, float nRadius, int nBirthrate, float nParticleLife, float dx, float dy, float dz, float nBirthSize, float r, float g, float b, float a){
    render_facade->createParticleSystem(shape, px, py, pz, nRadius, nBirthrate, nParticleLife, dx, dy, dz, nBirthSize, r, g, b, a);
}

void Motor3D::setAnimationActive(Node& n, bool f){
    render_facade->setAnimationActive(n,f);
}

/////////////////////////////////////////
// CLEAR METHODS
/////////////////////////////////////////

void Motor3D::resetFrameDelta(){
    render_facade->resetFrameDelta();
}

float Motor3D::getElapsedTime(){
    return render_facade->getTimeElapsed();
}

void Motor3D::clearNode(Node& n){
    render_facade->clearNode(n);
}

void Motor3D::clearInventory(){
    render_facade->insertLifes(Player::Instance()->getLifes());
}

void Motor3D::setMeshTexture(Node& n, std::string t){
    render_facade->setMeshTexture(n,t);
}

void Motor3D::setOpenedChest(Node& n){
    render_facade->setOpenedChest(n);
}

void Motor3D::setDebugEnemy(bool sn){
    render_facade->setdebugEnemies(sn);
}

void Motor3D::setDebugIA(bool sn){
    render_facade->setdebugIA(sn);
}

void Motor3D::playEatingEika(){
    render_facade->playEatingEika();
}

void Motor3D::playEquipEika(){
    render_facade->playEquipEika();
}