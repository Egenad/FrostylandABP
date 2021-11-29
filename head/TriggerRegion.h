#ifndef TRIGGERREGION_H
#define TRIGGERREGION_H

#include "Vector2f.h"

class TriggerRegion
{
    public:
        virtual ~TriggerRegion(){}
        virtual bool isTouching(Vector2f)const = 0;
        Vector2f* getPosition(){return tr_position;}
    protected:
        Vector2f* tr_position;
        float    tr_radius;
        float    tr_height;
};

#endif // TRIGGERREGION_H