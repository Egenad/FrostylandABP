#include "Vector2f.h"

Vector2f::Vector2f(){
	_x = -1;
	_y = -1;
}

Vector2f::Vector2f(float x, float y)
{
    //ctor
    _x = x;
    _y = y;
}

Vector2f::Vector2f(const Vector2f &v2){
    _x = v2.getX();
    _y = v2.getY();
}

Vector2f& Vector2f::operator=(Vector2f other){

    _x = other.getX();
    _y = other.getY();

    return *this;
}

Vector2f::~Vector2f()
{
    //dtor
}

void Vector2f::setX(float x){
    _x = x;
}

void Vector2f::setY(float y){
    _y = y;
}

float Vector2f::getX() const{
    return _x;
}

float Vector2f::getY() const{
    return _y;
}

void Vector2f::changeAllValues(float x, float y){
    _x = x;
    _y = y;
}