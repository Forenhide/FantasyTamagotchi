#include "SoundManager.h"
#include <iostream>


bool SoundManager::init(void) {

	loadSounds();
	setSounds();

	return true;
}

void SoundManager::loadSounds(void) {
	
	if (!buffers[SoundEffect::Click].loadFromFile("assets/sounds/Click.mp3")) throw std::runtime_error("btn clicked sound");
	if (!buffers[SoundEffect::Warning].loadFromFile("assets/sounds/Warning.wav")) throw std::runtime_error("warning sound");
	if (!buffers[SoundEffect::ItemUsed].loadFromFile("assets/sounds/ItemUsed.wav")) throw std::runtime_error("item used sound");
}

void SoundManager::setSounds(void) {
	
	for (int type = 0; type < SoundEffect::Count; ++type) {
		for (int sound = 0; sound < MAX_SOUNDS; ++sound) {
			int index = type * MAX_SOUNDS + sound;
			sounds[index].setBuffer(buffers[type]);

			if (type == SoundEffect::Click)			sounds[index].setVolume(5.f);
			else if (type == SoundEffect::Warning)  sounds[index].setVolume(4.f);
			else if (type == SoundEffect::ItemUsed) sounds[index].setVolume(5.f);
			else									sounds[index].setVolume(5.f);
		}
	}
}

void SoundManager::play(SoundEffect::Type effect) {

	int sound = nextSound[effect];
	int index = static_cast<int>(effect) * MAX_SOUNDS + sound;

	sounds[index].play();

	nextSound[effect] = (sound + 1) % MAX_SOUNDS;
}