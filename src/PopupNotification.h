#pragma once

#include <string>


enum class GameEvent { 
	STAT_CHANGE, 
	EVENT, 
	WARNING 
};

enum PositionAnchor { 
	BottomLeft, 
	TopCenter,  
	LeftCenter
};

struct Request {
	
	std::string message;
	GameEvent event;
	PositionAnchor pos;
};