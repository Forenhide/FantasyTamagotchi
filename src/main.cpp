#include "GameState.h" 
#include "Renderer.h" 
#include <windows.h> 

int main(void) { 
	
	GameState game; game_init(game); 
	Renderer renderer(game); 
	
	if (!renderer.init()) return -1; 
	
	while (renderer.is_open()) { 
		renderer.handle_events(); 
		renderer.render(); 
	}
	// call game update tick if not handled by events 

	return 0;
} 