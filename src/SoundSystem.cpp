#include "SoundSystem.h"
#include "fmod_extra.hpp"
#include "EventManager.h"
#include "Player.h"
#include <iostream>

///////////////////////////////////////////
//Class declaration methods
///////////////////////////////////////////

SoundSystem::SoundSystem(){

	volume = 1.0f;

	system = nullptr;
	lowLevelSystem = nullptr;
	masterBank = nullptr;
	stringsBank = nullptr;

	ERRCHECK(FMOD_Studio_System_Create(&system, FMOD_VERSION));
	ERRCHECK(FMOD_Studio_System_GetCoreSystem(system, &lowLevelSystem));
	
	ERRCHECK(FMOD_System_SetSoftwareFormat(lowLevelSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0));
	ERRCHECK(FMOD_System_SetOutput(lowLevelSystem, FMOD_OUTPUTTYPE_AUTODETECT));

	ERRCHECK(FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

	ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "../assets/audio/banks/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
	ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "../assets/audio/banks/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

	loadBank();
    
}
        
SoundSystem::~SoundSystem(){
	closeAudioEngine();
}
        
void SoundSystem::setVolume(float vol){
	volume = vol;
	for(long unsigned int i=0; i<soundEvents.size(); i++){
		soundEvents[i]->setVolume(volume);
	}
}

void SoundSystem::setSoundsVolume(float vol){

    for(long unsigned int i = 0; i<soundEvents.size(); i++){
		//std::cout<< "Cambio volumen -> "<< soundEvents[i]->getName() <<"\n";
		soundEvents[i]->setVolume(vol);	
	}

}

void SoundSystem::setListenerPosition(){
	 //Update listener position
	 auto* pl = Player::Instance();
	 
	 if(pl && pl->getY() != -1){
		 if(system != nullptr && pl != nullptr){

	        FMOD_3D_ATTRIBUTES attributes;

	        //Listener position (Camara)
	      
	        attributes.position.x = pl->getX();
	        attributes.position.y = pl->getY() + 60;
	        attributes.position.z = pl->getZ() - 80;

	        //Listener velocity
	        attributes.velocity = {0.f, 0.f, 0.f};

	        //The forwards orientation of the object.(Vector director - camara)
	    	attributes.forward.x = 0;
	    	attributes.forward.y = 0;
	    	attributes.forward.z = 1;
		    	
	        //The upwards orientation of the object.
	        attributes.up = {0.f,1.f, 0.f};

	        ERRCHECK(FMOD_Studio_System_SetListenerAttributes(system, 0, &attributes));
		}
	}
}

void SoundSystem::setParameterValue(std::string name, int p, float value) {
	bool done = false;

    for(long unsigned int i = 0; i<soundEvents.size() && !done; i++){
		if(soundEvents[i]->getName().compare(name) == 0){
			soundEvents[i]->setParameterValue(p, value);
			done = true;
		}
	}
}
        
void SoundSystem::update(){
	//Update listener position and orientation
    setListenerPosition();

    for(long unsigned int i=0; i<soundEvents.size(); i++){
        if(!soundEvents[i]->isPlaying() && soundEvents[i] != nullptr) {
            delete soundEvents[i];
            soundEvents[i] = nullptr;
            soundEvents.erase(soundEvents.begin() + i);
        }
    }

	ERRCHECK(FMOD_Studio_System_Update(system));
}

void SoundSystem::addListeners(){

}

void SoundSystem::loadBank(){

	//Load all events

	//Musica
		LOAD_EVENT(Frostyland,Musica/Menu);
		LOAD_SINGLE(Musica/Juego/Castillo2);
		LOAD_SINGLE(Musica/Juego/Batalla);
		//LOAD_SINGLE(Musica/Juego/Tienda);
		LOAD_SINGLE(Musica/GameOver);

	//Ambiente
		//LOAD_SINGLE(Ambientes/Juego/Castillo);
		//LOAD_SINGLE(Ambientes/Juego/Mar);
		LOAD_SINGLE(Ambientes/Juego/Nieve);
		//LOAD_SINGLE(Ambientes/Juego/Viento);

	//Diseño
		//LOAD_SINGLE(Diseño/Juego/PuzzleErroneo);
		LOAD_SINGLE(Diseño/Juego/PuzzleResuelto);
		LOAD_SINGLE(Diseño/Juego/VidaBaja);

		LOAD_SINGLE(Diseño/Menu/Navegar);
		LOAD_SINGLE(Diseño/Menu/SeleccionarJugar);
		LOAD_SINGLE(Diseño/Menu/SeleccionarOpcion);
		LOAD_SINGLE(Diseño/Menu/SeleccionarSalir);
		
	//Foleys
		LOAD_SINGLE(Foleys/Inventario/Abrir);
		LOAD_SINGLE(Foleys/Inventario/Cerrar);

		LOAD_SINGLE(Foleys/Objetos/Comer);
		LOAD_SINGLE(Foleys/Objetos/Comprar);	
		LOAD_SINGLE(Foleys/Objetos/Desequipar);	
		LOAD_SINGLE(Foleys/Objetos/Equipar);	
		LOAD_SINGLE(Foleys/Objetos/Llave);	
		LOAD_SINGLE(Foleys/Objetos/Moneda);	
		LOAD_SINGLE(Foleys/Objetos/Tirar);	

	//HardFX
		LOAD_SINGLE(HardFX/Objetos/Antorcha);

		LOAD_SINGLE(HardFX/Objetos/Armas/Espada/Impacto);
		LOAD_SINGLE(HardFX/Objetos/Armas/Espada/NoImpacto);
		LOAD_SINGLE(HardFX/Objetos/Armas/Martillo/ImpactoPersonaje);
		LOAD_SINGLE(HardFX/Objetos/Armas/Martillo/ImpactoSuelo);
		LOAD_SINGLE(HardFX/Objetos/Armas/Tirachinas/Impacto);
		LOAD_SINGLE(HardFX/Objetos/Armas/Tirachinas/Lanzamiento);

		LOAD_SINGLE(HardFX/Objetos/Caja);
		LOAD_SINGLE(HardFX/Objetos/Cofre/Abrir);
		LOAD_SINGLE(HardFX/Objetos/Placa);

		//LOAD_SINGLE(HardFX/Personaje/Pasos/Nieve);
		//LOAD_SINGLE(HardFX/Personaje/Pasos/Piedra);

		LOAD_SINGLE(HardFX/Puerta/Abrir);
		LOAD_SINGLE(HardFX/Puerta/Bloqueada);
		LOAD_SINGLE(HardFX/Puerta/Cerrar);
		LOAD_SINGLE(HardFX/Puerta/Desbloquear);
		LOAD_SINGLE(HardFX/KingPongo/Mareo);
		LOAD_SINGLE(HardFX/KingPongo/Rebotar);

	//Voces
		//LOAD_SINGLE(Voces/AI/Aldeano/Hablar);

		LOAD_SINGLE(Voces/AI/KingPongo/Atacar);
		LOAD_SINGLE(Voces/AI/KingPongo/Hablar);
		LOAD_SINGLE(Voces/AI/KingPongo/Morir);
		LOAD_SINGLE(Voces/AI/KingPongo/RDaño);
		LOAD_SINGLE(Voces/AI/KingPongo/Reir);

		LOAD_SINGLE(Voces/AI/Pgeneral/Atacar);
		LOAD_SINGLE(Voces/AI/Pgeneral/Morir);
		LOAD_SINGLE(Voces/AI/Pgeneral/RDaño);

		LOAD_SINGLE(Voces/AI/Psoldado/Atacar);
		LOAD_SINGLE(Voces/AI/Psoldado/Morir);
		LOAD_SINGLE(Voces/AI/Psoldado/RDaño);
		LOAD_SINGLE(Voces/AI/Psoldado/Dormir);
		
		//LOAD_SINGLE(Voces/AI/Reina/Hablar);

		LOAD_SINGLE(Voces/AI/Zorro/Atacar);
		LOAD_SINGLE(Voces/AI/Zorro/Morir);
		LOAD_SINGLE(Voces/AI/Zorro/RDaño);

		LOAD_SINGLE(Voces/Player/Atacar);
		LOAD_SINGLE(Voces/Player/Morir);
		LOAD_SINGLE(Voces/Player/RDaño);

}

void SoundSystem::stop(std::string name) {
	bool done = false;
	//Search the sound event
    for(long unsigned int i = 0; i<soundEvents.size() && !done; i++){
		if(soundEvents[i]->getName().compare(name) == 0){
			//stop the soundEvent
			soundEvents[i]->stop();
			done = true;
		}
	}
}

void SoundSystem::stop(SoundEvent* event) {
    //Stop the soundEvent
    if(event)
        event->stop();
}

void SoundSystem::unloadBank(){
	//Delete all soundEvents
	for(long unsigned int i=0; i<soundEvents.size(); i++){
		delete soundEvents[i];
		soundEvents[i] = nullptr;
	}

    soundEvents.clear();
    
    eventDescriptions.clear();
    //Delete banks
    for(auto bank : banks)
        ERRCHECK(FMOD_Studio_Bank_Unload(bank.second));
        
	banks.clear();
}

void SoundSystem::closeAudioEngine(){

	unloadBank();

    ERRCHECK(FMOD_Studio_Bank_Unload(stringsBank));
    ERRCHECK(FMOD_Studio_Bank_Unload(masterBank));
    
	ERRCHECK(FMOD_Studio_System_Release(system));

	system = nullptr;
	lowLevelSystem = nullptr;
	masterBank = nullptr;
	stringsBank = nullptr;
}

float SoundSystem::getVolume(){
	return volume;
}

bool SoundSystem::isPlaying(std::string name){
	//Return true if the event is playing

	bool playing = false;

	for(long unsigned int i = 0; i<soundEvents.size() && !playing; i++){
		if(soundEvents[i]->getName().compare(name) == 0){
			playing = true;
		}
	}

	return playing;
}


///////////////////////////////////////////
//Audio effect methods
///////////////////////////////////////////
SoundEvent* SoundSystem::createAudio(std::string name){
	//Create a SoundEvent
	auto descripter = eventDescriptions.find(name);
	SoundEvent* event = nullptr;

	if(descripter != eventDescriptions.end()){
		FMOD_STUDIO_EVENTINSTANCE* instance = nullptr;
		FMOD_Studio_EventDescription_CreateInstance(descripter->second, &instance);
		
		event = new SoundEvent(instance, name);
		event->setVolume(volume);
		soundEvents.push_back(event);

	}
	
	return event;
}

SoundEvent* SoundSystem::shootAudio(std::string name, int p, float value){
	SoundEvent* event = createAudio(name);

	//if p is possitive, set parameter in the soundevent
	if(event){
		if(p != -1){
			event->setParameterValue(p, value);
		}
		//Start playing
		event->start();
	}

	return event;
}

SoundEvent* SoundSystem::shoot3DAudio(std::string name, int p, float value, Vector3f pos){
	SoundEvent* event = createAudio(name);

	if(event){
		if(p != -1){
			event->setParameterValue(p, value);
		}
		//Set position
		event->setPosition(pos);	
		event->start();
	}

	return event;
}