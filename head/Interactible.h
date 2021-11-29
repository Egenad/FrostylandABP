#ifndef Interactible_H
#define Interactible_H

#include "Object.h"
#include <math.h>

class Interactible: public Object
{

    public:

        bool interactEntity(float ent_x, float ent_z){
            //Check distance between key and player

            int x = ps_x - ent_x;
            x = pow(x,2);
            int z = ps_z - ent_z;
            z = pow (z,2);
            int xz = x+z;

            int dist = sqrt(xz);

            if(dist <= m_radiusInteract){
                return true;
            }

            return false;
        }

        void setRadiusOfInteract(int r){
            m_radiusInteract = r;
        }

        virtual void playAudio(int) = 0;
    private:

        int m_radiusInteract;
};

#endif // Interactible_H