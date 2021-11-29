#pragma once
#ifndef ANIMARENDER_H
#define ANIMARENDER_H

#include "Globals.h"
#include "BaseGameEntity.h"
#include "AnimaEngine.h"
#include "Node.h"
#include "ABillboard.h"
#include "Estado.h"
#include <chrono>

using namespace m3D;

const struct keymap {
   char c;
   int keynum;
} myMap[] = {
   {'a', GLFW_KEY_A},
   {'s', GLFW_KEY_S},
   {'d', GLFW_KEY_D},
   {'w', GLFW_KEY_W},
   {'o', GLFW_KEY_O},
   {'p', GLFW_KEY_P},
   {'-', GLFW_KEY_ESCAPE},
   {'i', GLFW_KEY_I},
   {'6', GLFW_KEY_F6},
   {'5', GLFW_KEY_F5},
   {'4', GLFW_KEY_F4},
   {'3', GLFW_KEY_F3},
   {'2', GLFW_KEY_F2},
   {'1', GLFW_KEY_F1},

};

class AnimaRender{
	public:

        AnimaRender();
        virtual ~AnimaRender();

        //Class main methods

        void createCamara(float, float, float, float, float, float);
        void createLight(float, float, float, float, float, float);
        void resetFrameDelta();

        //Draw methods

        void beginScene(bool, bool, int, int, int, int);
        void drawScene();
        void drawLoadingScreen();
        void dibujarMenu(int);
        void dibujarGO();
        void drawEntity(float, float, float, float, char);
        void drawBillBoardEntity(Node&, Node&, float, float, float, EntityType);
        void drawDoor(Node&, float, float, float, bool, EntityType);
        void drawChest(Node&, float, float, float);
        void drawPlatform(Node&, float, float, float);
        void drawPlate(Node&, float, float, float);
        void drawBox(Node&, float, float, float);
        void drawInventory(int[],bool[],int,bool, bool, bool,float, float, int);
        void drawLine(float, float, float, float, float, float, char);
        void drawItemOptions(float, float, int, int);
        void drawTorch(Node&,float,float,float,int);
        void drawPainting(Node&,float,float,float,int);
        void drawBed(Node&,float,float,float,int);
        void drawChair(Node&,float,float,float,int);
        void drawChain(Node&,float,float,float,int);
        void drawPSD(Node&,float,float,float,int);
        void drawBox(Node&,float,float,float,int);
        void drawFlag(Node&,float,float,float,int);
        void drawColumn(Node&,float,float,float,int);
        void drawS1(Node&,float,float,float,int);
        void drawS2(Node&,float,float,float,int);
        void drawTable(Node&,float,float,float,int);
        void drawThrone(Node&,float,float,float,int);
        void drawBarrel(Node&,float,float,float,int);
        void drawSwordDeco(Node&,float,float,float,int);
        void drawLance(Node&,float,float,float,int);
        void drawPauseButtons(int);
        void drawGameOverMenu(int);
        void showMoney(int);
        void RenderDialog(int,EntityType);
        void RenderHistory(int);
        void RenderInventoryDescription();
        void RenderDatosEnemy(int);
        void RenderDatosPlayer();
        void drawInfoInteract(bool);
        void drawInfoControl(int);
        void drawEquipedWeapon(int);
        void drawObjectReceived(int);
        void playEatingEika();
        void playEquipEika();
        void redibujarOptions(int,int);
        void redibujarOptions(int);
        void loadTextures(int);
        void activateBillboards(Estado::StateType);
        void playAnimation(Node&);

        void clearNode();
        void clear();
        void clearScene();
        void clearDungeonMesh();

        void setRadius(float);
        void setTransition(bool);
        void setWindowSize(float,float);

        float getNow();
        float getTimeElapsed();
        float getDeltaTime();
        bool  isKeyDown(char);
        bool  isKeyReleased(char);

        //Controller
        int isJoystickPresent();
        void initJoystick();
        void checkJoystickAxes(float&, float&);
        bool onButtonPressed(char button);

        void setPositionEntity(Node&, float, float, float);
        void rotateEntity(Node&,float);
        void setBillboardActives();
        void setAnimationActive(Node&, bool);
        void setLoadingInactive(bool);
        void setMeshTexture(Node&, std::string);
        void setOpenedChest(Node&);
        void clearNode(Node&);
        void cleanDialogText();
        void setdebugIA(bool);
        void setdebugEnemies(bool sn);

        //Main methods
        int  sigueActivo();
        void inicializarDungeon(int);
        void updatePlCamera(float, float, float, float, float, float);
        void insertLifes(int);
        void setDebugDrawer(ADebug*);
        void setLODdecoration(Node&, bool);
        void createParticleSystem(std::string shape, float px, float py, float pz, float nRadius, int nBirthrate, float nParticleLife, float dx, float dy, float dz, float nBirthSize, float r, float g, float b, float a);

    private:

        //Must-to variables

        AnimaEngine* device;
        ANode* camera;

        //Timers

        float frameDeltaTime;
        float then;
        std::chrono::time_point<std::chrono::system_clock> chr_start, chr_then;
        float chr_delta;
        float now;
        float timeelapsed;
        int lastop;

        //LoadingScreen
        float timerLS;
        bool created;
        int actualSprite;

        //Meshes

        ANode* dungeon_mesh;
        ANode* castle_ground;
        ANode* eika3d;

        //Billboards

        ABillboard* menu_background;
        ABillboard* gameover_background;
        ABillboard* continue_button;
        ABillboard* exit_button;
        ABillboard* options_button;
        ABillboard* newgame_button;
        ABillboard* input_buttons;
        ABillboard* equiped_weapon;
        ABillboard* exitmenu_button;
        ABillboard* restart_button;
        std::vector<ABillboard*> lifes;
        std::vector<ABillboard*> weapons;
        std::vector<ABillboard*> keys;
        std::vector<ABillboard*> food;
        ABillboard* option_back;
        ABillboard* option_use;
        ABillboard* option_erase;
        ABillboard* option_selector;
        ABillboard* item_selector;
        ABillboard* inventory_background;
        ABillboard* dialog_background;
        ABillboard* dialog_icon;
        ABillboard* interact_info;
        ABillboard* control_info;
        ABillboard* loadingscreen;
        ABillboard* enemy_infobg;
        ABillboard* object_received;
        ABillboard* bg_object_received;
        ABillboard* bg_eika_debug;
        ABillboard* enemy_selector;
        ABillboard* options;
        ABillboard* music;
        ABillboard* musiconoff;
        ABillboard* volumebill;
        ABillboard* resolution;
        ABillboard* r_left;
        ABillboard* r_right;
        ABillboard* v_up;
        ABillboard* v_down;
        ABillboard* b_pause;
 
        //Text

        AText* resolution_text;
        AText* dialog_text0;
        AText* dialog_text1;
        AText* dialog_text2;
        AText* dialog_text3;
        AText* enemy_text1;
        AText* enemy_text2;
        AText* enemy_text3;
        AText* enemy_text4;
        AText* enemy_text5;
        AText* inventory_desc0;
        AText* inventory_desc1;
        AText* inventory_desc2;
        AText* inventory_desc3;
        AText* money_text;
        AText* pl_debug1;
        AText* pl_debug2;
        AText* pl_debug3;
        AText* pl_debug4;
        AText* pl_debug5;

        //Joystick - Controller

        int axesCount,buttonCount;
        const float *axes;
        const unsigned char *buttons;
};




#endif // ANIMARENDER_H