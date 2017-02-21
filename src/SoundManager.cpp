#include "SoundManager.h"

using namespace irrklang;

#define ASSET_DIRECTORY "../../assets/"

SoundManager::SoundManager(float volume) {
	se = createIrrKlangDevice();
	setBaseVolume(volume);
}

SoundManager::~SoundManager() {
	if (se) se->drop();
}

ISound* SoundManager::playSound(const char* path) {
	if (se)
		return se->play2D(path, true, false, true);
	return NULL;
}
void SoundManager::stopAllSounds() {
	if (se)	se->stopAllSounds();
}

void SoundManager::playMenuMusic() {
	se->play2D(ASSET_DIRECTORY "Stringed Disco.mp3", true, false, true);
}
void SoundManager::playDrivingMusic() {
	se->play2D(ASSET_DIRECTORY "Neon and Kickboxing.mp3", true, false, true);
}

float SoundManager::getBaseVolume() {
	return baseVolume;
}
void SoundManager::setBaseVolume(float volume) {
	baseVolume = volume;
	if(se) se->setSoundVolume(volume);
}
