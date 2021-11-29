#include "CalculateShortPath.h"
#include "Enemy.h"
#include "Dungeon.h"
#include "AStar.h"
#include "Fox.h"

CalculateShortPath::CalculateShortPath(){}

CalculateShortPath::~CalculateShortPath(){}

bool CalculateShortPath::execute(int id){
	return calcule(id); 
}

bool CalculateShortPath::calcule(int id){

    auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    Fox* fox = dynamic_cast<Fox*> (npc);

    if(fox && fox->getPathList() == nullptr){

        int aux = d->wichRoomIAmAt(fox->getX(), fox->getZ());
        Room* r2 = d->getRoomPerID(aux); //Room where the enemy is at this moment

        if(r2->getAlert() <= 1){
            if(d->getGrafo() != nullptr){
                Room* r = d->getRoomWithMoreAlert(); //Room where the enemy wants to go

                //std::cout << r->getCenterX() << "," <<  r->getCenterZ() << "\n";

                int rcx = (int)r->getCenterX();
                int rcz = (int)r->getCenterZ();

                float rcxf = rcx;
                float rczf = rcz;

                int rcx2 = (int)r2->getCenterX();
                int rcz2 = (int)r2->getCenterZ();

                float rcxf2 = rcx2;
                float rczf2 = rcz2;

                AStar astar(*d->getGrafo());

                PathfindingList* result = astar.run(Vector2f(rcxf2, rczf2),Vector2f(rcxf, rczf));
                if(result != nullptr){
                    fox->setPathList(*result);
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }

	return true;
}
