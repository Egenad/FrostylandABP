#include "Sequence.h"

Sequence::Sequence(){

}

Sequence::~Sequence(){
	Task* pd;
	for(std::list<Task*>::iterator it = childrens.begin(); it != childrens.end(); ++it) {
		pd = *it;
		delete pd;
		pd = nullptr;
	}
	childrens.clear();
}

bool Sequence::execute(int id){

	if(childrens.size() > 0){
        
        current_child = childrens.begin();	
        
        for (std::list<Task*>::iterator i = childrens.begin(); i != childrens.end(); ++i) {
        	if(!(*i)->execute(id)){
        		return false;
        	}
        }
    }
	return true;
}