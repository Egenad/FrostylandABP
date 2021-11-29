#pragma once
#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>
#include "Estado.h"
#include "Seal_Normal.h"
#include "Seal_Quest.h"
#include "Enemy.h"
#include "Room.h"
#include "Door.h"
#include "DragBox.h"
#include "Queen.h"
#include "GrafoP.h"

class Dungeon : public Estado
{
    public:
        static Dungeon* Instance();

        //Class methods
        void handleInput();
        void draw();
        void update();
        void drawDebug();
        void clearState();
        void addEvents();
        void addEnemy(Enemy*);
        void addDragBox(DragBox*);
        void addVillager(Villager*);
        void restart();
        void openInventory();
        void loadDungeon();
        float calculeDistance(Vector2f, Vector2f);
        void setLOD(Decoration*, float);

        //LOD

        //void checkLOD();

        //Triggers
        void lockDoors(bool);
        void lockOneDoor(EntityType, bool);
        void playerFalls();
        void createEnemies();
        void createVillagers();
        void deleteEnemiesInPlayerRoom();
        void createChest(EntityType);
        void setBossAnimation(bool);
        void startBossBattle();
        void setKPongoBattle();
        void setAnimations(int,int);
        void createQueen();

        //Setters
        void setLoadSaveData(bool);
        void setResetPlayer();
        void setLevelMediaY(int);
        void setLoaded(bool);
        void setLevel(int);
        void changeLevel();
        void setChangeTextures(bool);
        void setGrafo(GrafoP);
        void checkObjRec();
        void startObjRec();
        void startTransition();
        void setGameOver(bool);

        //Getters
        int getLevelMediaY();
        GrafoP* getGrafo();
        Room* getRoomWithMoreAlert();
        int  getLevel();
        int  getLastLevel();
        int wichRoomIAmAt(float, float);
        int getDoorsSize();
        Enemy* getEnemyPerID(int);
        std::vector<int> getEnemiesPerRoomID(int);
        std::vector<Door*>& getDoors();
        std::vector<Room*>& getRooms();
        std::vector<Enemy*>& getEnemies();
        std::vector<Villager*>& getSeals();
        std::vector<DragBox*>& getDragBoxes();
        Room*  getRoomPerID(int);
        int  getRoomIDPerCenterInt(Vector2f);
        Door*  getDoorPerID(int);
        Door*  getDoorPerRoomsID(int, int);
        Door*  getClosestDoor(Vector2f);
        Door*  getClosestDoor(Vector2f, int);
        Villager*  getClosestNSeal(Vector2f);
        Villager*  getClosestQSeal(Vector2f);
        Door* getDoorPerIndex(long unsigned int i);
        DragBox* getDragBoxPerID(int);
        Vector3f getClosestPinguPos(int);
        Vector3f getNearestObst(float, float);
        int getFPS();

    protected:
        Dungeon();
        virtual ~Dungeon();
    private:
        std::vector<Villager*> seals;
        std::vector<Enemy*> enemies;
        std::vector<Room*> rooms;
        std::vector<Door*> doors;
        std::vector<DragBox*> dragboxes;

        GrafoP* dungeonGraph;

        bool e_released;
        int level;
        int lastlevel;
        bool erased;
        bool resetPlayer;
        float then;
        bool init_boss_animation;
        float animation_clock;
        bool loaded;
        bool loadsave;
        bool loadlevel;
        bool debug;
        bool IAdebug;
        bool changeTextures;
        int  levelmediaY;
        float objrec;
        bool transition;
        int tnumber;
        float radius;
        bool mt_up;
        bool gameOver;
        int fps;
        int fps2;
        float fps_clock;
        long unsigned int enemyselec;
};

#endif // DUNGEON_H
