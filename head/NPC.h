#ifndef NPC_H
#define NPC_H

#include "Node.h"
#include "BaseGameEntity.h"
#include "Task.h"
#include "Selector.h"
#include "Vector2f.h"

class NPC : public BaseGameEntity
{
    
    public:

        //Getters

        float getVelocityX(){
            return velocidadX;
        }

        float getVelocityZ(){
            return velocidadZ;
        }

        float getMaxVel(){
            return maxvel;
        }
       
        m3D::Node& getNode(){
            return soldier;
        }

        m3D::Node& getShadow(){
            return shadow;
        }
        
        Vector2f getSize(){
            return Vector2f(width,height);
        }

        //Setters
        void  setVelocityX(float vx){
            velocidadX = vx;
        }
        void  setVelocityZ(float vz){
            velocidadZ = vz;
        }

        void  setMaxVel(float mv){
            maxvel = mv;
        }

        void  setPosition(float x, float y, float z){
            ps_x = x;
            ps_y = y;
            ps_z = z;
        }
        
        void resetPosition(){
            ps_x = ini_x;
            ps_y = ini_y;
            ps_z = ini_z;
        }

        void setIniPosition(float x, float y, float z){
            ini_x = x;
            ini_y = y;
            ini_z = z;
        }

        //Identifier comparation method
        bool compareID(int id){
        	if(id == getID()) return true;
        	return false;
        }

        virtual void playAudio(int) = 0;

    protected:

        //Characteristics
        
        float velocidadX;
        float velocidadZ;
        float width;
        float height;
        float maxvel;
        float ini_x;
        float ini_y;
        float ini_z;
        
        //Object variables
        
        m3D::Node soldier;
        m3D::Node shadow;
        Selector* selector_task;
};

#endif // NPC_H
