#include "game_state.h" 
#include "renderer.h" 
#include <windows.h> 

int main(void) { 
	
	sf::Clock clock;
	GameState game; game_init(game); 
	Renderer renderer(game); 
	
	if (!renderer.init()) return -1; 
	
	while (renderer.is_open()) { 
		
		float dt = clock.restart().asSeconds();

		renderer.handle_events(); 
		tick(game, dt);
		renderer.update(dt);
		renderer.render(); 
	}

	return 0;
} 