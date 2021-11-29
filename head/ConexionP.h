#ifndef CONEXIONP_H
#define CONEXIONP_H

#include "Vector2f.h"

class ConexionP
{
    public:
    	ConexionP();
        ConexionP(Vector2f, Vector2f, float);
        ConexionP(const ConexionP&);
        ConexionP& operator=(ConexionP);
        virtual ~ConexionP();
        void setFromNode(Vector2f);
        void setToNode(Vector2f);
        void setCost(float);
        Vector2f getFromNode() const;
        Vector2f getToNode() const;
        float getCost() const;
    private:
        Vector2f n1;
        Vector2f n2;
        float coste; //cost of the connection
};

#endif // CONEXIONP_H