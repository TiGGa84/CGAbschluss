#pragma once

#include "irrKlang.h"

#pragma comment(lib, "irrKlang.lib")

class SoundManager {
public:
	SoundManager(float volume = 0.4f);
	virtual ~SoundManager();

	irrklang::ISound* playSound(const char* path);
	void stopAllSounds();
	void playMenuMusic();
	void playDrivingMusic();

	float getBaseVolume();
	void setBaseVolume(float volume);
protected:
	irrklang::ISoundEngine* se;
	float baseVolume;
};