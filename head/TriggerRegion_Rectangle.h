#ifndef TRIGGERREGION_RECTANGLE_H
#define TRIGGERREGION_RECTANGLE_H

#include "TriggerRegion.h"

class TriggerRegion_Rectangle: public TriggerRegion
{
    public:

        virtual ~TriggerRegion_Rectangle(){
            if(tr_position){
                delete tr_position;
                tr_position = nullptr;
            }
        }

    	TriggerRegion_Rectangle(Vector2f center, float radius){
    		tr_position = new Vector2f(center.getX(), center.getY());
    		tr_radius = radius;
            tr_height = -1;
    	}

        TriggerRegion_Rectangle(Vector2f center, float radius, float height){
            tr_position = new Vector2f(center.getX(), center.getY());
            tr_radius = radius;
            tr_height = height;
        }

        bool isTouching(Vector2f ent_p)const{

            if(tr_height == -1){
            	if(ent_p.getX() > (tr_position->getX() - tr_radius) && ent_p.getX() < (tr_position->getX() + tr_radius)){
            		if(ent_p.getY() > (tr_position->getY() - tr_radius) && ent_p.getY() < (tr_position->getY() + tr_radius)){
            			return true;
            		}
            	}
            }else{
                if(ent_p.getX() > (tr_position->getX() - tr_radius) && ent_p.getX() < (tr_position->getX() + tr_radius)){
                    if(ent_p.getY() > (tr_position->getY() - tr_height) && ent_p.getY() < (tr_position->getY() + tr_height)){
                        return true;
                    }
                }
            }

        	return false;
        }
};

#endif // TRIGGERREGION_RECTANGLE_H