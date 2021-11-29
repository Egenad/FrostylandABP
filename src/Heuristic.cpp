#include "Heuristic.h"
#include <math.h>
#include <iostream>

Heuristic::Heuristic(){

}

Heuristic::~Heuristic(){

}

float Heuristic::getHeuristic(Vector2f n1, Vector2f n2){
	float lvalue = 0;

	float _x1 = n1.getX();
	float _y1 = n1.getY();
	float _x2 = n2.getX();
	float _y2 = n2.getY();

	lvalue = sqrt(pow(_x1 - _x2, 2) + pow(_y1 - _y2, 2));

	return lvalue;
}