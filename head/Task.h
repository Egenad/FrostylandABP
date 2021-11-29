#ifndef TASK_H
#define TASK_H

#include <list>

class Task
{
    public:
        //If task was done successfully then it returns true, otherwise it will return false
        virtual bool execute(int)=0; 
        virtual ~Task(){}
        void addTask(Task* task){
        	childrens.push_back(task);
        }
    protected:
        //A task can have childs (more tasks) in order to do a composite instruction
        std::list<Task*> childrens;
        std::list<Task*>::iterator current_child;
};

#endif // TASK_H