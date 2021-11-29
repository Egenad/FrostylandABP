
/* ================================================================================================== */
/* Cosas extra para FMOD                                                                              */
/* ================================================================================================== */

#ifndef _FMOD_EXTRA_HPP
#define _FMOD_EXTRA_HPP

#include <iostream>
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"

inline void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}


#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

#define LOAD_EVENT(bank, event) \
    if (banks.find(#bank) == banks.end()) { \
        banks.insert(std::pair<std::string, FMOD_STUDIO_BANK*>(#bank, nullptr)); \
        ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "../assets/audio/banks/"#bank".bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &banks[#bank])); \
    } \
    if (eventDescriptions.find(#event) == eventDescriptions.end()) { \
        eventDescriptions.insert(std::pair<std::string, FMOD_STUDIO_EVENTDESCRIPTION*>(#event, nullptr)); \
        ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/"#event,  &eventDescriptions[#event])); \
}

#define LOAD_SINGLE(event) \
    if (eventDescriptions.find(#event) == eventDescriptions.end()) { \
        eventDescriptions.insert(std::pair<std::string, FMOD_STUDIO_EVENTDESCRIPTION*>(#event, nullptr)); \
        ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/"#event,  &eventDescriptions[#event])); \
}

#endif

