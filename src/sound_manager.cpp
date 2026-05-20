#include "sound_manager.h"
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
	if (!buffers[SoundEffect::ItemBought].loadFromFile("assets/sounds/ItemBought.ogg")) throw std::runtime_error("item bought sound");
}

void SoundManager::setSounds(void) {
	
	for (int type = 0; type < SoundEffect::Count; ++type) {
		for (int sound = 0; sound < MAX_SOUNDS; ++sound) {
			sounds.emplace_back(buffers[type]);

			sf::Sound& s = sounds.back();

			if (type == SoundEffect::Click)			s.setVolume(5.f);
			else if (type == SoundEffect::Warning)  s.setVolume(10.f);
			else if (type == SoundEffect::ItemUsed) s.setVolume(30.f);
			else if (type == SoundEffect::ItemBought) s.setVolume(30.f);
		}
	}
}

void SoundManager::play(SoundEffect::Type effect) {

	int sound = nextSound[effect];
	int index = static_cast<int>(effect) * MAX_SOUNDS + sound;

	sounds[index].play();

	nextSound[effect] = (sound + 1) % MAX_SOUNDS;
}