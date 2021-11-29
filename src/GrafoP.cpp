#include "GrafoP.h"
#include <iostream>
GrafoP::GrafoP(std::vector<ConexionP> param)
{
    //ctor
    for(long unsigned int i=0; i<param.size(); i++){
    	grafo.push_back(param[i]);
    }
}

GrafoP::~GrafoP()
{
    //dtor
}

GrafoP::GrafoP(const GrafoP& g){
	std::vector<ConexionP> ggrafo = g.getGrafo();
	for(long unsigned int i=0; i<ggrafo.size(); i++){
		ConexionP c = ggrafo[i];
		grafo.push_back(c);
	}
}

std::vector<ConexionP> GrafoP::getConnections(Vector2f s){
	
	std::vector<ConexionP> conexiones;

	for(long unsigned int i=0; i<grafo.size(); i++){
		Vector2f n1 = grafo[i].getFromNode();
		if(n1.getX() == s.getX() && n1.getY() == s.getY()){
			conexiones.push_back(grafo[i]);
		}
	}

	return conexiones;
}

std::vector<ConexionP> GrafoP::getGrafo() const{
	return grafo;
}

void GrafoP::drawGraph(){
	for(long unsigned int i=0; i<grafo.size(); i++){
		Vector2f n1 = grafo[i].getFromNode();
		Vector2f n2 = grafo[i].getToNode();
		std::cout << "(" << n1.getX() << "," << n1.getY() << ") (" << n2.getX() << "," << n2.getY() << ")\n";
	}
}

bool GrafoP::nodeInGraph(Vector2f s){

	int _x = (int)s.getX();
	int _y = (int)s.getY();

	for(long unsigned int i=0; i<grafo.size(); i++){
		Vector2f n1 = grafo[i].getFromNode();
		Vector2f n2 = grafo[i].getToNode();
		if((n1.getX() == _x && n1.getY() == _y) || (n2.getX() == _x && n2.getY() == _y)){
			return true;
		}
	}

	return false;
}