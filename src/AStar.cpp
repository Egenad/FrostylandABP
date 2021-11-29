#include "AStar.h"
#include "Heuristic.h"
#include <iostream>
AStar::AStar(GrafoP g)
{
    //ctor
    openList = nullptr;
    closedList = nullptr;
    resultList = nullptr;
    grafo = new GrafoP(g);
}

AStar::~AStar()
{
    //dtor

    delete grafo;
    
    if(openList != nullptr){
        delete openList;
        openList = nullptr;
    }
    if(closedList != nullptr){
        delete closedList;
        closedList = nullptr;
    }
    if(resultList != nullptr){
        delete resultList;
        resultList = nullptr;
    }
}

PathfindingList* AStar::run(Vector2f start, Vector2f goal){
    //inicializacion
    NodeRecord startRecord;
    startRecord.node = start; //le asignamos el inicio
    startRecord.costSoFar = 0;
    startRecord.estimatedTotalCost = 0; //heuristic.estimate(start);

    //std::cout << "Start: " << startRecord.node.getX() << "," << startRecord.node.getY() << "\n";
    //std::cout << "End: " << goal.getX() << "," << goal.getY() << "\n\n";

    NodeRecord current; //nodo en el que me encuentro
    NodeRecord endNode; //nodo final desde el que me encuentro

    //inicializacion listas
    openList = new PathfindingList();
    openList->add(startRecord);
    //std::cout << "Añado a openList: " << startRecord.node.getX() << "," << startRecord.node.getY() << std::endl;
    closedList = new PathfindingList();

    resultList = new PathfindingList();

    //iteramos por todos los nodos abiertos
    while(!openList->isEmpty()){
        //buscamos el elemento de la lista que tenga el estimated cost mas pequenyo
        current = openList->findSmallestElement(); //aqui vamos a tener que usar el valor heuristico
        //std::cout << "\nNodo actual: " << current.node.getX() << "," << current.node.getY() << "\n\n";
        if(current.estimatedTotalCost != -1){
            if(current.node.getX() != goal.getX() || current.node.getY() != goal.getY()){
                
                std::vector<ConexionP> connections = grafo->getConnections(current.node);
            
                //para borrar nodos de la lista --> enemies.erase(enemies.begin()+j);

                for(long unsigned int i=0;i<connections.size();i++){

                    //std::cout << connections.size() << std::endl;

                    //std::cout << "Conexion to " << connections[i].getToNode().getX() << "," << connections[i].getToNode().getY() << "\n";
                    endNode.node = connections[i].getToNode(); //le pasas 1 nodo y te devuelve el otro
                    endNode.costSoFar = current.costSoFar + connections[i].getCost();
                    endNode.connection = connections[i];
                    endNode.estimatedTotalCost = 0;

                    if(!closedList->contains(endNode)){
                        
                        if(!resultList->contains(current)){
                            resultList->add(current);
                        }

                        NodeRecord endNodeRecord;
                        float endNodeHeuristic = 0;

                        if(openList->contains(endNode)){
                            endNodeRecord = openList->find(endNode);

                            if(endNodeRecord.costSoFar <= endNode.costSoFar)
                                endNodeHeuristic = endNodeRecord.estimatedTotalCost - endNodeRecord.costSoFar;   
                        }else{
                            endNodeRecord = endNode;
                            Heuristic eu;
                            endNodeHeuristic = eu.getHeuristic(endNode.node, current.node);
                            //std::cout << "Heuristica de la conexion: " << endNodeHeuristic << "\n";
                        }
                        
                        endNodeRecord.costSoFar = endNode.costSoFar;
                        endNodeRecord.connection = endNode.connection;
                        endNodeRecord.estimatedTotalCost = endNode.costSoFar + endNodeHeuristic;

                        if(!openList->contains(endNodeRecord)){
                            //std::cout << "Añado a openList: " << endNodeRecord.node.getX() << "," << endNodeRecord.node.getY() << std::endl;
                            openList->add(endNodeRecord);
                        }
                    }
                }
                
                openList->remove(current);
                //std::cout << "Añado a closedList: " << current.node.getX() << "," << current.node.getY() << std::endl;
                closedList->add(current);
                
            }else{
                resultList->add(current);
                break;
            }
        }
        //std::cout << current.node.getX() << std::endl;
    }
    if(current.node.getX() == goal.getX() && current.node.getY() == goal.getY()){
        //std::cout << "Resultado:\n";
        //resultList->draw();
        return resultList;
    }else{
        return nullptr;
    }
}

 /*

    def pathfindAStar(graph, start, end, heuristic):
        //This structure is used to keep track of the information we need for each node
        struct NodeRecord:
            node
            connection
            costSoFar
            estimatedTotalCost

        //Initialize the record for the start node
        startRecord = new NodeRecord()
        startRecord.node = start
        startRecord.connection = None
        startRecord.costSoFar = 0
        startRecord.estimatedTotalCost = heuristic.estimate(start)

        //Initialize the open and closed lists
        open = PathfindingList()
        open += startRecord
        closed = PathfindingList()


        //Iterate through processing each node
        while length(open) > 0:
            //Find the smallest element in the open list (using the estimatedTotalCost)
            //the NodeRecord structure in the list with the lowest costSoFar value.
            current = open.smallestElement()
            
            //If it is the goal node, then terminate
            if current.node == goal: break

            //Otherwise get its outgoing connections
            connections = graph.getConnections(current)

            //Loop through each connection in turn
            for connection in connections:

                //Get the cost estimate for the end node
                endNode = connection.getToNode()
                endNodeCost = current.costSoFar + connection.getCost()

                //If the node is closed we may have to skip, or remove it from the closed list.
                if closed.contains(endNode):

                    //Here we find the record in the closed list corresponding to the endNode.
                    endNodeRecord = closed.find(endNode)

                    //If we didn’t find a shorter route, skip
                    if endNodeRecord.costSoFar <= endNodeCost:
                        continue;

                    //Otherwise remove it from the closed list
                    closed -= endNodeRecord

                    //We can use the node’s old cost values to calculate its heuristic without calling the possibly expensive heuristic function
                    endNodeHeuristic = endNodeRecord.estimatedTotalCost - endNodeRecord.costSoFar

                //Skip if the node is open and we’ve not found a better route
                else if open.contains(endNode):

                    //Here we find the record in the open list corresponding to the endNode.
                    endNodeRecord = open.find(endNode)

                    //If our route is no better, then skip
                    if endNodeRecord.costSoFar <= endNodeCost:
                        continue;

                    //We can use the node’s old cost values to calculate its heuristic without calling the possibly expensive heuristic function
                    endNodeHeuristic = endNodeRecord.cost - endNodeRecord.costSoFar
                
                //Otherwise we know we’ve got an unvisited node, so make a record for it
                else:
                    endNodeRecord = new NodeRecord()
                    endNodeRecord.node = endNode

                    //We’ll need to calculate the heuristic value using the function, since we don’t have an existing record to use
                    endNodeHeuristic = heuristic.estimate(endNode)

                //We’re here if we need to update the node. Update the cost, estimate and connection
                endNodeRecord.cost = endNodeCost
                endNodeRecord.connection = connection
                endNodeRecord.estimatedTotalCost = endNodeCost + endNodeHeuristic

                //And add it to the open list
                if not open.contains(endNode):
                    open += endNodeRecord

            //We’ve finished looking at the connections for the current node, so add it to the closed list and remove it from the open list
            open -= current
            closed += current


        //We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
        if current.node != goal:
            //We’ve run out of nodes without finding the goal, so there’s no solution
            return None

        else:
            //Compile the list of connections in the path
            path = []
            //Work back along the path, accumulating connections
            while current.node != start:
                path += current.connection
                current = current.connection.getFromNode()

            //Reverse the path, and return it
            return reverse(path)
    */



    //OTRO ALGORITMO DE WIKIPEDIA
    /*
    function A_Star(start, goal)
    // The set of nodes already evaluated
    closedSet := {}

    // The set of currently discovered nodes that are not evaluated yet.
    // Initially, only the start node is known.
    openSet := {start}

    // For each node, which node it can most efficiently be reached from.
    // If a node can be reached from many nodes, cameFrom will eventually contain the
    // most efficient previous step.
    cameFrom := an empty map

    // For each node, the cost of getting from the start node to that node.
    gScore := map with default value of Infinity

    // The cost of going from start to start is zero.
    gScore[start] := 0

    // For each node, the total cost of getting from the start node to the goal
    // by passing by that node. That value is partly known, partly heuristic.
    fScore := map with default value of Infinity

    // For the first node, that value is completely heuristic.
    fScore[start] := heuristic_cost_estimate(start, goal)

    while openSet is not empty
        current := the node in openSet having the lowest fScore[] value
        if current = goal
            return reconstruct_path(cameFrom, current)

        openSet.Remove(current)
        closedSet.Add(current)

        for each neighbor of current
            if neighbor in closedSet
                continue        // Ignore the neighbor which is already evaluated.

            // The distance from start to a neighbor
            tentative_gScore := gScore[current] + dist_between(current, neighbor)

            if neighbor not in openSet  // Discover a new node
                openSet.Add(neighbor)
            else if tentative_gScore >= gScore[neighbor]
                continue;

            // This path is the best until now. Record it!
            cameFrom[neighbor] := current
            gScore[neighbor] := tentative_gScore
            fScore[neighbor] := gScore[neighbor] + heuristic_cost_estimate(neighbor, goal)
    */
