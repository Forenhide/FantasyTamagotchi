#pragma once

#include <string>


enum class GameEvent { 
	STAT_CHANGE, 
	EVENT, 
	WARNING,
	ITEM_USED
};

enum PositionAnchor { 
	BottomLeft, 
	TopCenter,  
	LeftUp,
	LeftCenter,
	LeftBottom,
	BottomCenter
};

struct Request {
	
	std::string message;
	GameEvent event;
	PositionAnchor pos;
};