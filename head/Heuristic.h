#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Vector2f.h"

class Heuristic
{
    public:
        Heuristic();
        virtual ~Heuristic();
        float getHeuristic(Vector2f, Vector2f);
};

#endif // ASTAR_H