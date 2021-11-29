#include "AudioManager.h"
#include "SoundSystem.h"
#include <iostream>

AudioManager* AudioManager::Instance(){
    static AudioManager pinstance;
    return &pinstance;
}

AudioManager::AudioManager(){
	//ctor
	soundsystem = new SoundSystem();
}

AudioManager::~AudioManager()
{
    //dtor
}

void AudioManager::update(){
	soundsystem->update();
}

void AudioManager::close(){
	soundsystem->closeAudioEngine();
}

void AudioManager::clearInstance(){
	delete soundsystem;
    soundsystem = nullptr;
}

bool AudioManager::isPlaying(std::string name){
	return soundsystem->isPlaying(name);
}

void AudioManager::stop(std::string name){
	soundsystem->stop(name);
}

void AudioManager::stop(SoundEvent* event){
	soundsystem->stop(event);
}

SoundEvent* AudioManager::shootAudio(std::string name, int parameter, float value){
	return soundsystem->shootAudio(name, parameter, value);
}

SoundEvent* AudioManager::shoot3DAudio(std::string name, int parameter, float value, Vector3f pos){
	return soundsystem->shoot3DAudio(name, parameter, value, pos);
}

void AudioManager::setParameterValue(std::string name, int parameter, float value){
	soundsystem->setParameterValue(name, parameter, value);
}

void AudioManager::setVolume(float v){
	soundsystem->setVolume(v);
}

void AudioManager::setSoundsVolume(float vol){
	soundsystem->setSoundsVolume(vol);
}

float AudioManager::getVolume(){
	return soundsystem->getVolume();
}