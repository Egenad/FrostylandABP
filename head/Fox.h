#ifndef FOX_H
#define FOX_H

#include "Enemy.h"
#include "PathfindingList.h"

class Fox: public Enemy
{
    public:
        Fox(Vector3f);
        virtual ~Fox();
        //Main Methods
        void recalculatePath();
        void playAudio(int);

        //Update
        void  updateEntity();
        void  executeTask();
        void  updateAnimation();

        //Getters
        PathfindingList* getPathList();
        Vector3f getLastPosition();
        //Setters
        void setPathList(PathfindingList);
        void removePathList();
    private:
    	void generateBehaviorTree();
    	PathfindingList* pathlist;
        Vector3f lastpos;
};

#endif // FOX_H