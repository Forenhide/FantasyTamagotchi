#pragma once

#include <SFML/Audio.hpp>
#include <vector>


constexpr int MAX_SOUNDS{3};

namespace SoundEffect {
	
	enum Type {
		
		Click = 0,
		Warning,
		ItemUsed,
		Birthday,
		ItemBought,
		Count
	};
}

struct SoundManager {

	sf::SoundBuffer buffers[SoundEffect::Count];
	std::vector<sf::Sound> sounds;

	int nextSound[SoundEffect::Count] = {0};

	bool init(void);
	void loadSounds(void);
	void setSounds(void);
	void play(SoundEffect::Type effect);
};