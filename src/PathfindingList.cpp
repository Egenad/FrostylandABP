#include "PathfindingList.h"

PathfindingList::PathfindingList()
{
    //ctor

}

PathfindingList::~PathfindingList(){}

PathfindingList::PathfindingList(const PathfindingList& path){
	std::vector<NodeRecord> nodel = path.getNodeList();
	for(long unsigned int i=0; i<nodel.size(); i++){
		nodeList.push_back(nodel[i]);
	}
}

void PathfindingList::add(NodeRecord r){
	nodeList.push_back(r);
}

bool PathfindingList::isEmpty(){
	if(nodeList.size() == 0){
		return true;
	}
	return false;
}

NodeRecord PathfindingList::findSmallestElement(){
	NodeRecord error;

	float aux = -1;

	if(nodeList.size() != 0){

		aux = nodeList[0].estimatedTotalCost;

		for(long unsigned int i=0; i<nodeList.size(); i++){
			if(nodeList[i].estimatedTotalCost < aux){
				aux = nodeList[i].estimatedTotalCost;
			}
		}
	}

	if(aux != -1){
		for(long unsigned int i=0; i<nodeList.size(); i++){
			if(aux == nodeList[i].estimatedTotalCost){
				return nodeList[i];
			}
		}
	}
	
	return error;
}

void PathfindingList::remove(NodeRecord r){
	for(long unsigned int i=0; i<nodeList.size(); i++){
		if(nodeList[i].node.getX() == r.node.getX() && nodeList[i].node.getY() == r.node.getY()){
            nodeList.erase(nodeList.begin()+i);
		}
	}
}

void PathfindingList::draw(){
	for(long unsigned int i=0; i<nodeList.size(); i++){
		std::cout << nodeList[i].node.getX() << "," << nodeList[i].node.getY() << "\n";
	}
}

bool PathfindingList::contains(NodeRecord r){
	bool contained = false;

	for(long unsigned int i=0; i<nodeList.size(); i++){
		if(nodeList[i].node.getX() == r.node.getX() && nodeList[i].node.getY() == r.node.getY()){
            contained = true;
		}
	}

	return contained;
}

NodeRecord PathfindingList::find(NodeRecord r){
	for(long unsigned int i=0; i<nodeList.size(); i++){
		if(nodeList[i].node.getX() == r.node.getX() && nodeList[i].node.getY() == r.node.getY()){
            return nodeList[i];
		}
	}

	NodeRecord smallest;
	smallest.costSoFar = -1;
	smallest.estimatedTotalCost = -1;

	return smallest;
}

std::vector<NodeRecord> PathfindingList::getNodeList() const{
	return nodeList;
}