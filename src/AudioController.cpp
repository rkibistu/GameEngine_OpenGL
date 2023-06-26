#include "stdafx.h"
#include "AudioController.h"

#include <iostream>

AudioController::AudioController() {

	if (FMOD::System_Create(&_fmodSystem) != FMOD_OK) {
		//nu avem sunete, vom afisa un mesaj de eroare
		std::cout << "FMOD BAD!\n";
	}
	else {
		std::cout << "FMOD Initialized!\n";
		_fmodSystem->init(36, FMOD_INIT_NORMAL, NULL);//initializat la 36 de canale
	}

	AddSound(1, "Resources/Sounds/toc.wav");
}

void AudioController::PlayTest() {

	_fmodSystem->playSound(_sounds[SoundClips::TOC], 0, false, 0);
}

void AudioController::AddSound(unsigned int id, std::string filepath) {

	FMOD::Sound* temp;
	_sounds.insert({ id,temp });
	_fmodSystem->createSound(filepath.c_str(), FMOD_LOOP_OFF, 0, &_sounds[id]); //FMOD_LOOP_OFF inseamna
}
