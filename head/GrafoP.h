#ifndef GRAFOP_H
#define GRAFOP_H

#include "ConexionP.h"
#include <vector>

class GrafoP
{
    public:
        GrafoP(std::vector<ConexionP>);
        GrafoP(const GrafoP&);
        virtual ~GrafoP();
        std::vector<ConexionP> getConnections(Vector2f);
        std::vector<ConexionP> getGrafo() const;
        void drawGraph();
        bool nodeInGraph(Vector2f);
    private:
        std::vector<ConexionP> grafo;

};

#endif // GRAFOP_H