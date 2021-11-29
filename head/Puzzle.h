#ifndef PUZZLE_H
#define PUZZLE_H

#include "BaseGameEntity.h"
#include <vector>

class Puzzle
{
    public:
        Puzzle();
        virtual ~Puzzle();
        //Getters
        bool getResolved() const;
        std::vector<EntityFlag> getConditions();
        //Setters
        void setResolved(bool);
        void addCondition(EntityFlag);
        //Class methods
        void checkConditions(int);
        void playAudio(int);
    private:
    	std::vector<EntityFlag> conditions;
        bool resolved;
};

#endif // SMEMORY_H