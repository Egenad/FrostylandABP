#ifndef MOTOR3D_H
#define MOTOR3D_H

//#include "RenderFacade.h"
//#include "IrrInputFacade.h"
#include "AnimaRender.h"
#include "Node.h"
#include "Vector3f.h"
#include "Vector2f.h"
#include "BaseGameEntity.h"
#include "Estado.h"

namespace m3D{
    class Motor3D
    {
        public:
            //Instance
            static Motor3D* Instance();

            //Public destructor
            void clearInstance();

            //Draw
            void resetFrameDelta();
            void beginScene(bool,bool,int,int,int,int);
            void drawScene();
            void dibujarMenu(int);
            void dibujarMenuPause(int);
            void dibujarMenuGameOver(int);
            void drawEntity(Node&, float, float, float, float,char);
            void drawBillBoardEntity(Node&,Node&,float,float,float, EntityType);
            void drawDoor(Node&, float, float, float, bool, EntityType);
            void drawChest(Node&, float, float, float);
            void drawPlatform(Node&, float, float, float);
            void drawInteractible(Node&, float, float, float, EntityType);
            void drawLine(Vector3f, Vector3f,char);
            void drawInventory(int[],bool[],int,bool,bool, bool, float,float, int);
            void drawItemOptions(float, float, int, int);
            void drawDescription();
            void drawDialog(int, EntityType);
            void drawDatosEnemy(int);
            void drawDatosPlayer();
            void clearDungeonMesh();
            void drawInfoInteract(bool);
            void drawInfoControl(int);
            void drawEquipedWeapon(int);
            void drawLoadingScreen();
            void drawDecoration(Node&,float,float,float,int,int);
            void drawReceived(int);
            void setDebugEnemy(bool);
            void playEatingEika();
            void playEquipEika();

            void setRadius(float);
            void setTransition(bool);
            void redibujarOptions(int,int);
            void redibujarOptions(int);
            void clearNode(Node&);
            void clearInventory();
            void clear();
            void clearScene();
            void cleanDialogText();

            void loadTextures(int);

            void setPositionEntity(Node&, float, float, float);
            void rotateEntity(Node&, float);
            void setWindowSize(float,float);

            float getNow();
            float getDeltaTime();
            float getElapsedTime();

            //Main methods
            int  sigueActivo();
            void inicializarDungeon(int);
            void updatePlCamera(Vector3f, Vector3f);
            void insertLifes(int);
            void activarBillboards(Estado::StateType);
            void setMeshTexture(Node&,std::string);
            void setLoadingInactive(bool);
            void setDebugDrawer(ADebug*);
            void setOpenedChest(Node&);
            void playAnimation(Node&);
            void createParticleSystem(std::string shape, float px, float py, float pz, float nRadius, int nBirthrate, float nParticleLife, float dx, float dy, float dz, float nBirthSize, float r, float g, float b, float a);
            void setAnimationActive(Node&, bool);
            void setLODdeco(Node&, bool);
            void setDebugIA(bool);
            //Input
            bool onKeyPressed(char);
            bool isconectedJoystick();
            void initController();
            void checkJoystickAxes(float&, float&);
            bool onButtonPressed(char button);

        protected:
            //Ctor & Dtor
            Motor3D();
            virtual ~Motor3D();
        private:        
            //Variables
            //RenderFacade* render_facade;
            //IrrInputFacade* input_facade;
            AnimaRender* render_facade;
            
    };
}

#endif // MOTOR3D_H
