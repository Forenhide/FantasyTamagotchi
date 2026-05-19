#include "game_state.h" 
#include "renderer.h" 
#include <windows.h> 

int main(void) { 
	
	GameState game; game_init(game); 
	Renderer renderer(game); 
	
	if (!renderer.init()) return -1; 
	
	while (renderer.is_open()) { 
		renderer.handle_events(); 
		renderer.render(); 
	}

	return 0;
} 