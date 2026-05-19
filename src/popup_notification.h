#pragma once

#include <string>


enum class GameEvent { 
	STAT_CHANGE, 
	EVENT, 
	WARNING,
	ITEM_USED,
	ITEM_BOUGHT
};

enum PositionAnchor { 
	BottomLeft, 
	TopCenter,  
	LeftUp,
	LeftCenter,
	LeftBottom,
	BottomCenter,
	ShopBottomLeft
};

struct Request {
	
	std::string message;
	GameEvent event;
	PositionAnchor pos;
};