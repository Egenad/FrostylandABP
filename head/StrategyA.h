#ifndef STRATEGYA_H
#define STRATEGYA_H

#include "Strategy.h"

class StrategyA : public Strategy{
public:
    StrategyA();
    virtual ~StrategyA();
    void execute(int);
private:
    void makeDamage(int, int);
    float timer;
    float confused_timer;
    int option;
    bool change;
    int collision;
    bool confused;
    int number;
    float init_pos;
    float a_time;
};

#endif /* STRATEGYA_H */