#include "save_system.h"
#include <fstream>
#include <iostream>
#include <limits>


void save_game(const GameState& g) {
	
	std::ofstream file ("save.txt");

	if (!file.is_open()) {
		std::cout << "Failed to save game\n";
		return;
	}

	file << g.health << '\n';
	file << g.energy << '\n';
	file << g.hungerNum << '\n';
	file << g.boredomNum << '\n';
	file << g.age << '\n';
	file << g.balance << '\n';
	file << g.level << '\n';
	file << g.experience << '\n';
	file << g.nextLevelExp << '\n';
	file << g.isIll << '\n';

	// Inventory size
	file << g.inventory.size() << '\n';

	// Inventory items
	for (const Item& item : g.inventory) {
		file << item.name << '\n';
	}

	file.close();

	std::cout << "Game saves\n";
}

bool load_game(GameState& g) {
	
	std::ifstream file ("save.txt");

	if (!file.is_open()) {
		std::cout << "No save file found\n";
		return false;
	}

	file >> g.health;
	file >> g.energy;
	file >> g.hungerNum;
	file >> g.boredomNum;
	file >> g.age;
	file >> g.balance;
	file >> g.level;
	file >> g.experience;
	file >> g.nextLevelExp;
	file >> g.isIll;

	pet_translate_hunger(g);
	pet_translate_boredom(g);

	// Inventory
	int inventorySize;
	file >> inventorySize;

	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	g.inventory.clear();

	for (size_t i = 0; i < inventorySize; ++i) {
		
		std::string itemName;
		if (std::getline(file, itemName)) {
			for (const Item& item : g.items) {
				if (item.name == itemName) {
					g.inventory.push_back(item);
					break;
				}
			}
		}
	}

	file.close();

	std::cout << "Game loaded\n";

	return true;
}

void delete_save(void) {
	
	if (std::remove("save.txt") == 0) {
		std::cout << "Save deleted\n";
	} else {
		std::cout << "Failed to delete save\n";
	}
}