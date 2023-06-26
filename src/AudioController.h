#pragma once

#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"

#include <string>
#include <unordered_map>

enum SoundClips {

	TOC = 1,
};

class AudioController {
public:
	AudioController();
	~AudioController();
	
	void AddSound(unsigned int id, std::string filepath);

	void PlaySound(SoundClips clip);
	void PlayTest();

private:
	FMOD::System* _fmodSystem;

	FMOD::Sound* _testSound;
	std::unordered_map<unsigned int, FMOD::Sound*> _sounds;
};