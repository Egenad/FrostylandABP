#include "EventManager.h"
#include <iostream>


EventManager* EventManager::Instance(){
    static EventManager pinstance;
    return &pinstance;
}

EventManager::EventManager()
{
    Listeners = new std::map<Type, std::function<void()>>();
    AudioListeners = new std::map<Type, std::function<void()>>();
}

void EventManager::clearInstance(){
    close();
    delete Listeners;
    Listeners = nullptr;
    delete AudioListeners;
    AudioListeners = nullptr;
}

EventManager::~EventManager()
{
    //dtor
}

void EventManager::close(){
    while(!eventQueue.empty()){
        eventQueue.pop();
    }

    while(!eventAudioQueue.empty()){
        eventAudioQueue.pop();
    }

    Listeners->clear();
    AudioListeners->clear();
    
}

void EventManager::cleanMap(){
    Listeners->clear();
}

void EventManager::clearEvents(){
    while(!eventQueue.empty()){
        eventQueue.pop();
    }
}

void EventManager::addEvent(Event event){
    eventQueue.push(event);
}

void EventManager::addAudioEvent(Event event){
    eventAudioQueue.push(event);
}

void EventManager::runManager(){
    dispatchEvents();
}

void EventManager::dispatchEvents(){

    while(!eventAudioQueue.empty()){

        Event event = eventAudioQueue.front();

        auto process = AudioListeners->find(event.getType());

        if(process != AudioListeners->end()){
            process->second();
        }

        eventAudioQueue.pop();
    }

    while(!eventQueue.empty()){

        Event event = eventQueue.front();

        auto process = Listeners->find(event.getType());

        if(process != Listeners->end()){
            process->second();
        }

        eventQueue.pop();

    }
}

void EventManager::addListener(Type type, std::function<void()> funct){
    EventManager::Listeners->insert(Pair(type, funct));
}

void EventManager::addAudioListener(Type type, std::function<void()> funct){
    AudioListeners->insert(Pair(type, funct));
}
