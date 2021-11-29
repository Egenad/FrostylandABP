#ifndef FACTORIANIVEL_H
#define FACTORIANIVEL_H

#include <vector>
#include "tinystr.h"
#include "tinyxml.h"
#include "Villager.h"
#include "Enemy.h"
#include "Room.h"
#include "Door.h"

class FactoriaNivel
{
    public:
        static FactoriaNivel* Instance();
        void readLevel(std::string, std::vector<Room*>&, std::vector<Door*>&, std::vector<Enemy*>&, std::vector<Villager*>&);
        void saveGame(int);
        void destroySaveData();
        void loadPlayer();
        void loadGame(int, std::vector<Room*>& rooms, std::vector<Door*>& doors, std::vector<Enemy*>& enemies, std::vector<Villager*>& seals);
        bool saveDataAvailable();
        void putPlayerPosition(int, int);
        void clearInstance();
    protected:
        FactoriaNivel();
        virtual ~FactoriaNivel();
    private:
        TiXmlDocument* doc;
};

#endif // FACTORIANIVEL_H
