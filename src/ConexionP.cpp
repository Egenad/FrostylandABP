#include "ConexionP.h"
#include <iostream>
ConexionP::ConexionP(){
	n1.setX(-1);
	n1.setY(-1);
	n2.setX(-1);
	n2.setY(-1);
	coste = -1;
}

ConexionP::ConexionP(Vector2f nodo1, Vector2f nodo2, float c)
{
    //ctor
    n1.setX(nodo1.getX());
    n1.setY(nodo1.getY());
    n2.setX(nodo2.getX());
    n2.setY(nodo2.getY());
    coste = c;
}

ConexionP& ConexionP::operator=(ConexionP other){
	n1 = other.getFromNode();
	n2 = other.getToNode();
	coste = other.getCost();

	return *this;
}

ConexionP::~ConexionP()
{
    //dtor 
}

ConexionP::ConexionP(const ConexionP &cp){
	n1 = cp.getFromNode();
	n2 = cp.getToNode();
	coste = cp.getCost();
}

Vector2f ConexionP::getFromNode() const{
	return n1;
}

Vector2f ConexionP::getToNode() const{
	return n2;
}

float ConexionP::getCost() const{
	return coste;
}

void ConexionP::setFromNode(Vector2f f){
	n1.setX(f.getX());
	n1.setY(f.getY());
}

void ConexionP::setToNode(Vector2f t){
	n2.setX(t.getX());
	n2.setY(t.getY());
}

void ConexionP::setCost(float c){
	coste = c;
}
