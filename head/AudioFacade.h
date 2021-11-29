#pragma once

#include "Vector3f.h"
#include "BaseGameEntity.h"
#include "SoundEvent.h"
#include <string>

class AudioFacade{

    public:

        AudioFacade() {}
        virtual ~AudioFacade(){}

        //Main methods
        virtual void update() = 0;
        virtual void closeAudioEngine() = 0;
        virtual void loadBank() = 0;
        virtual void unloadBank() = 0;
        virtual void addListeners() = 0;
        virtual void stop(std::string) = 0;
        virtual void stop(SoundEvent*) = 0;
        virtual SoundEvent* shootAudio(std::string, int, float) = 0;
        virtual SoundEvent* shoot3DAudio(std::string, int, float, Vector3f) = 0;
        virtual void setParameterValue(std::string, int, float) = 0;
        
        //Setters
        virtual void setVolume(float vol) = 0;
        virtual void setSoundsVolume(float) = 0;
        virtual void setListenerPosition() = 0;

        //Getters
        virtual float getVolume() = 0;
        virtual bool isPlaying(std::string) = 0;

        void setListener(EntityFlag& newListener) {
            listener = &newListener;
        }

        EntityFlag* getListener() {
            return listener;
        }

    private:

        EntityFlag* listener = nullptr;

};