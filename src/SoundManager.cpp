#include "SoundManager.h"

using namespace irrklang;

#define ASSET_DIRECTORY "../../assets/"

SoundManager::SoundManager(float volume) {
	se = createIrrKlangDevice();
	baseVolume = volume;
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
	if (se)
		se->stopAllSounds();
}

void SoundManager::playMenuMusic() {
	ISound* snd = se->play2D(ASSET_DIRECTORY "Stringed Disco.mp3", true, false, true);
	if (snd) snd->setVolume(baseVolume);
}
void SoundManager::playDrivingMusic() {
	ISound* snd = se->play2D(ASSET_DIRECTORY "Neon and Kickboxing.mp3", true, false, true);
	if (snd) snd->setVolume(baseVolume);
}

float SoundManager::getBaseVolume() {
	return baseVolume;
}
void SoundManager::setBaseVolume(float volume) {
	baseVolume = volume;
}
