#include "SoundEvent.h"
#include <iostream>

SoundEvent::SoundEvent(FMOD_STUDIO_EVENTINSTANCE* instance, std::string n){
	volume = 1.0f;
	soundInstance = instance;
	name = n;
}

SoundEvent::~SoundEvent(){
	FMOD_Studio_EventInstance_Release(soundInstance);
	soundInstance = nullptr;
}

void SoundEvent::start(){
	if(soundInstance!=nullptr){
        FMOD_Studio_EventInstance_Start(soundInstance);
	}
}
        
void SoundEvent::stop(){
	if(soundInstance != nullptr)
		FMOD_Studio_EventInstance_Stop(soundInstance, FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_IMMEDIATE);
}

std::string SoundEvent::getName(){
	return name;
}

void SoundEvent::setParameterValue(int parameter, float value){
	if(soundInstance != nullptr){
		if(parameter == 0){
			FMOD_Studio_EventInstance_SetParameterByName(soundInstance, "Random", value, false);
		}else if(parameter == 1){
			FMOD_Studio_EventInstance_SetParameterByName(soundInstance, "Level", value, false);
		}else if(parameter == 2){
			FMOD_Studio_EventInstance_SetParameterByName(soundInstance, "Life", value, false);
		}
	}

}

void SoundEvent::setVolume(float vol){
	if(vol>=0 && vol<=1){
        FMOD_Studio_EventInstance_SetVolume(soundInstance, vol * volume);
	}
}

void SoundEvent::setPosition(Vector3f pos){

	FMOD_3D_ATTRIBUTES attributes;
    
    attributes.position.x = pos.getX();
    attributes.position.y = pos.getY();
    attributes.position.z = pos.getZ();

    attributes.velocity.x = 0;
    attributes.velocity.y = 0;
    attributes.velocity.z = 0;

    attributes.forward.x = 0;
    attributes.forward.y = 0;
    attributes.forward.z = -1;

    attributes.up.x = 0;
    attributes.up.y = 1;
    attributes.up.z = 0;

	FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
}

bool SoundEvent::isPlaying(){
	if(soundInstance!=nullptr){
        FMOD_STUDIO_PLAYBACK_STATE state;
        FMOD_Studio_EventInstance_GetPlaybackState(soundInstance, &state);
        if(state == FMOD_STUDIO_PLAYBACK_PLAYING || state == FMOD_STUDIO_PLAYBACK_STARTING){
            return true;
        }
    }
	return false;
}