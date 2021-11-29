#include "Vector3f.h"

Vector3f::Vector3f(){
    _x = -1;
    _y = -1;
    _z = -1;
}

Vector3f::Vector3f(float x, float z)
{
    //ctor
    _x = x;
    _y = 0.f;
    _z = z;
}

Vector3f::Vector3f(float x, float y, float z)
{
    //ctor
    _x = x;
    _y = y;
    _z = z;
}

Vector3f::Vector3f(const Vector3f& v3){
    _x = v3.getX();
    _y = v3.getY();
    _z = v3.getZ();
}

Vector3f::~Vector3f()
{
    //dtor
}

void Vector3f::setX(float x){
    _x = x;
}

void Vector3f::setY(float y){
    _y = y;
}

void Vector3f::setZ(float z){
    _z = z;
}

float Vector3f::getX() const{
    return _x;
}

float Vector3f::getY() const{
    return _y;
}

float Vector3f::getZ() const{
    return _z;
}

void Vector3f::changeAllValues(float x, float y, float z){
    _x = x;
    _y = y;
    _z = z;
}