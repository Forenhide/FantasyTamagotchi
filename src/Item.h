#ifndef ITEM_H
#define ITEM_H

#include <string>


enum class ItemType {
	
	FOOD,
	MEDICINE,
	TOY
};

struct Item {
	
	ItemType type;
	std::string name;
	int stat;
	int price;
};

#endif