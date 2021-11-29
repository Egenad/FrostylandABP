#pragma once

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "AudioFacade.h"
#include <string>

class AudioManager
{
    public:

        static AudioManager* Instance();
        void clearInstance();
        void update();
        void close();
        void stop(std::string);
        void stop(SoundEvent*);
        SoundEvent* shootAudio(std::string, int, float);
        SoundEvent*  shoot3DAudio(std::string, int, float, Vector3f);
        void setParameterValue(std::string, int, float);
        void setSoundsVolume(float);
        bool isPlaying(std::string);
        void setVolume(float);
        float getVolume();

    protected:

        AudioManager();
        virtual ~AudioManager();

    private:
        AudioFacade* soundsystem;

};

#endif // AUDIOMANAGER_H