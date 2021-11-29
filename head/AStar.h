#ifndef ASTAR_H
#define ASTAR_H

#include "GrafoP.h"
#include "PathfindingList.h"

class AStar
{
    public:
        AStar(GrafoP);
        virtual ~AStar();
        PathfindingList* run(Vector2f, Vector2f);
    private:
        GrafoP* grafo;
        PathfindingList* openList;
        PathfindingList* closedList;
        PathfindingList* resultList;
};

#endif // ASTAR_H