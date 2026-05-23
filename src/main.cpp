#include "game_state.h" 
#include "renderer.h" 
#include "save_system.h"


int main(void) { 
	
	sf::Clock clock;
	GameState game; 
	game_init(game); 
	load_game(game);
	Renderer renderer(game); 

	float autoSaveTimer = 0.f;
	
	if (!renderer.init()) return -1; 
	
	while (renderer.is_open()) { 
		
		float dt = clock.restart().asSeconds();

		autoSaveTimer += dt;

		if (autoSaveTimer >= 30.f) {
			if (game.scene != Scene::Death) save_game(game);
			autoSaveTimer = 0.f;
		}

		renderer.handle_events(); 

		if (game.scene != Scene::Menu) tick(game, dt);
		
		renderer.update(dt);
		renderer.render(); 
	}

	return 0;
} 

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();
}
#endif