#pragma once
#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <map>

using namespace std;

#include "Vector3f.h"
#include <fmod_studio.h>
#include <fmod_studio.hpp>

#include "SoundEvent.h"
#include "AudioFacade.h"
#include <vector>

typedef std::pair<std::string, SoundEvent*> AudioPair;

class SoundSystem : public AudioFacade
{
    public:
        SoundSystem();   
        ~SoundSystem();
        
        //Getters
        float getVolume();

        //Setters
        void setVolume(float vol);
        void setSoundsVolume(float);
        void setListenerPosition();
        void setParameterValue(std::string , int , float );
        
        //Main methods
        void stop(std::string name);
        void stop(SoundEvent*);
        void update();
        void closeAudioEngine();
        void loadBank();
        void unloadBank();
        void addListeners();

        bool isPlaying(std::string);

        SoundEvent* createAudio(std::string);
        SoundEvent* shootAudio(std::string, int, float);
        SoundEvent* shoot3DAudio(std::string, int, float, Vector3f);

    private:
        string banksPath;

        FMOD_STUDIO_SYSTEM*    system;
        FMOD_SYSTEM*           lowLevelSystem;

        FMOD_STUDIO_BANK* masterBank;
        FMOD_STUDIO_BANK* stringsBank;

        map<string, FMOD_STUDIO_BANK*> banks;
        map<string, FMOD_STUDIO_EVENTDESCRIPTION*> eventDescriptions;
        std::vector<SoundEvent*> soundEvents;

        float volume;
};

#endif // SOUNDSYSTEM_H