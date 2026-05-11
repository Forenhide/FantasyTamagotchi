#pragma once

#include <SFML/Audio.hpp>


constexpr int MAX_SOUNDS{8};

namespace SoundEffect {
	
	enum Type {
		
		Click = 0,
		Warning,
		ItemUsed,
		Birthday,
		Count
	};
}

struct SoundManager {

	sf::SoundBuffer buffers[SoundEffect::Count];
	sf::Sound sounds[SoundEffect::Count * MAX_SOUNDS];

	int nextSound[SoundEffect::Count] = {0};

	bool init(void);
	void loadSounds(void);
	void setSounds(void);
	void play(SoundEffect::Type effect);
};