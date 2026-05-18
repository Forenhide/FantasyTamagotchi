#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>


struct ItemEffect {
	
	int value;
	enum class StatType { HEALTH,
						  HUNGER,
						  MOOD,
						  ENERGY };
	StatType type;
};

struct Item {
	
	std::string name;
	int price;
	int index;
	std::vector<ItemEffect> itemEffects;

	float currentScale{0.45f};
};

#endif