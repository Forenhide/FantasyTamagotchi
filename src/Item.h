#ifndef ITEM_H
#define ITEM_H

#include <string>


enum class ItemType {
	
	FOOD,
	MEDICINE,
	MOOD,
	ENERGY,
	UNIVERSAL
};

struct Item {
	
	ItemType type;
	std::string name;
	int value;
	int price;

	float currentScale{1.f};
};

#endif