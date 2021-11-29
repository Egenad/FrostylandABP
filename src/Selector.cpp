#include "Selector.h"

Selector::Selector(){

}

Selector::~Selector(){
	//childrens.clear();
	Task* pd;
	for(std::list<Task*>::iterator it = childrens.begin(); it != childrens.end(); ++it) {
		pd = *it;
		delete pd;
		pd = nullptr;
	}
	childrens.clear();
}

bool Selector::execute(int id){

	if(childrens.size() > 0){
        
        current_child = childrens.begin();	
        
        for (std::list<Task*>::iterator i = childrens.begin(); i != childrens.end(); ++i) {
        	if((*i)->execute(id)){
        		return true;
        	}
        }
    }
	return false;
}