#ifndef TRIGGER_H
#define TRIGGER_H

#include "BaseGameEntity.h"
#include "TriggerRegion_Rectangle.h"

class Trigger: public BaseGameEntity
{
    public:
        Trigger(){};
        virtual ~Trigger(){};
        virtual void Try() = 0;
        bool isToBeRemoved()const{return m_removeFromGame;}
        bool isActive(){return m_trActive;}
        void addRectangularTriggerRegion(Vector2f center, float radius){
            m_regionOfInfluence = new TriggerRegion_Rectangle(center, radius);
        }
        void addRectangularTriggerRegion(Vector2f center, float radius, float height){
            m_regionOfInfluence = new TriggerRegion_Rectangle(center, radius, height);
        }
    protected:
        void setInactive(){m_trActive = false;}
        void setActive(){m_trActive = true;}
        void setToRemove(bool r){m_removeFromGame = r;}
        bool isTouchingTrigger(Vector2f ent_pos)const{
            return m_regionOfInfluence->isTouching(ent_pos);
        };

        TriggerRegion* m_regionOfInfluence;
    private:	
        bool m_removeFromGame;
        bool m_trActive;
};

#endif // TRIGGER_H