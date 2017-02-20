#include "SoundManager.h"

using namespace std;
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
	if (snd) snd->setVolume(0.4f);
}

float SoundManager::getBaseVolume() {
	return baseVolume;
}
void SoundManager::setBaseVolume(float volume) {
	baseVolume = volume;
}
