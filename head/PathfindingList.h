#ifndef PATHFINDINGLIST_H
#define PATHFINDINGLIST_H

#include <vector>
#include "ConexionP.h"
#include <iostream>

struct NodeRecord{
    NodeRecord(){
        costSoFar = -1;
        estimatedTotalCost = -1;
    }
    virtual ~NodeRecord(){

    }
    NodeRecord(const NodeRecord& n){
        node = n.node;
        connection = n.connection;
        costSoFar = n.costSoFar;
        estimatedTotalCost = n.estimatedTotalCost;
    }
    NodeRecord& operator=(NodeRecord other){
        std::swap(node, other.node);
        std::swap(connection, other.connection);
        std::swap(costSoFar, other.costSoFar);
        std::swap(estimatedTotalCost, other.estimatedTotalCost);
        return *this;
    }
	Vector2f node;
    ConexionP connection;
    float costSoFar; //coste desde el start hasta él
    float estimatedTotalCost; //calculado con la heuristica
};

class PathfindingList
{
    public:
        PathfindingList();
        PathfindingList(const PathfindingList&);
        virtual ~PathfindingList();
        void add(NodeRecord);
        bool isEmpty();
        void draw();
        NodeRecord findSmallestElement();
        void remove(NodeRecord);
        bool contains(NodeRecord);
        NodeRecord find(NodeRecord);
        std::vector<NodeRecord> getNodeList() const;
    private:
    	std::vector<NodeRecord> nodeList;
};

#endif // PATHFINDINGLIST_H