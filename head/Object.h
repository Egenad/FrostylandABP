#ifndef Object_H
#define Object_H

#include "Motor3D.h"
#include "BaseGameEntity.h"
#include "Node.h"

class Object:  public BaseGameEntity
{
    public:

        //Getters

        float getRadius(){
            return r;
        }

        m3D::Node& getNode(){
            return object;
        }
        char getColor(){
            return color;
        }

        float getX(){
            return ps_x;
        }

        float getY(){
            return ps_y;
        }

        float getZ(){
            return ps_z;
        }

        //Setters

        void  setPosition(float x, float y, float z){
            ps_x = x;
            ps_y = y;
            ps_z = z;
        }

        void  setPosition(float x, float z){
            ps_x = x;
            ps_z = z;
        }

        void setColor(char c){
            color = c;
        }

        

    protected:
        //Characteristics
        float r;
        char  color;

        //Object variables
        m3D::Node object;

};

#endif // Object_H