#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
#include <map>
#include <queue>
#include <functional>

typedef std::pair<Type, std::function<void()>> Pair;

class EventManager
{

    public:

        static EventManager* Instance();
        void close();
        void clearEvents();
        void addEvent(Event);
        void addAudioEvent(Event);
        void cleanMap();
        void addListener(Type, std::function<void()>);
        void addAudioListener(Type, std::function<void()>);
        void runManager();
        void clearInstance();
    protected:

        EventManager();
        virtual ~EventManager();

    private:
        
        void dispatchEvents();
        std::queue<Event> eventQueue;                       //Los eventos son de un tipo
        std::map<Type, std::function<void()>> *Listeners;   //Los listeners contienen un tipo de evento a la cual se le asigna una funcion
        std::queue<Event> eventAudioQueue;                       
        std::map<Type, std::function<void()>> *AudioListeners;
};

#endif // EVENTMANAGER_H
