#ifndef MOVETONODE_H
#define MOVETONODE_H

#include "Task.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Enemy.h"
#include "Door.h"

class MoveToNode: public Task
{
    public:
        bool execute(int);
        MoveToNode();
        virtual ~MoveToNode();
        void seek(Door*, Enemy*, Vector3f, int);
    private:
        bool move(int);
        Vector2f wActual;
        Vector2f wNext;
        bool toDoorW1;
        bool toDoorW2;
        long unsigned int index;
        int doorindex;
        bool toclose;
        bool tolock;
        bool inverse;
        float timer;
};

#endif // MOVETONODE_H