#ifndef SOUNDEVENT_H
#define SOUNDEVENT_H

using namespace std;

#include "Vector3f.h"
#include <fmod_studio.h>
#include <fmod_studio.hpp>
#include <string>

class SoundEvent
{
    public:

        SoundEvent(FMOD_STUDIO_EVENTINSTANCE*, std::string);
        ~SoundEvent();

        void start();
        
        void stop();
        
        void setParameterValue(int, float);

        void setVolume(float vol);

        void setPosition(Vector3f pos);

        bool isPlaying();

        std::string getName();

    protected:

        FMOD_STUDIO_EVENTINSTANCE* soundInstance;
        float volume;
        std::string name;
};

#endif // SOUNDEVENT_H