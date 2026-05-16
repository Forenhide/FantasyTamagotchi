#include "GameState.h"
#include <iostream>
#include <random>

#pragma region Static
static int get_level(int value, int max, int levels) {

	return (value * (levels - 1) / max);
}

static int get_inverted_level(int value, int max, int levels) {
	
	return (levels - 1) - get_level(value, max, levels);
}

static int get_random_number(void) {
	
	std::random_device rd;
	std::mt19937 g(rd());

	std::uniform_int_distribution<int> randomNum(0, 1000);

	return randomNum(g);
}
#pragma endregion

#pragma region Game Initialization / Update
void game_init(GameState& g) {

	// Scene
	g.scene = Scene::Pet;
	
	// Pet
	g.name = "Tony Tony Chopper";
	g.hunger = HUNGER[4];
	g.mood = MOOD[4];
	g.art = "😀";
	g.health = 100;
	g.energy = 30;
	g.age = 0;
	g.hungerNum = 0;
	g.boredomNum = 0;
	g.illnessStart = 0;
	g.illnessEnd = 0;
	g.tickCount = 1;
	g.isAlive = true;
	g.birthday = false;
	g.isIll = false;

	// Inventory & Shop
	g.items = { {ItemType::MEDICINE, "Healing Potion", 30, 5},
				{ItemType::MOOD, "Joy Potion", 25, 7},
				{ItemType::UNIVERSAL, "Universal Potion", 50, 35},
				{ItemType::FOOD, "Mochi", 12, 5},
				{ItemType::FOOD, "Pineapple", 35, 12},
				{ItemType::MOOD, "Teddy Bear", 7, 3},
				{ItemType::MOOD, "Tetris", 11, 4},
				{ItemType::FOOD, "Tiramisu", 20, 7},
				{ItemType::MOOD, "Yo-Yo", 5, 2} };

	g.inventoryItems = { {ItemType::MEDICINE, "Healing Potion", 30, 5},
						 {ItemType::MOOD, "Joy Potion", 25, 7},
						 {ItemType::UNIVERSAL, "Universal Potion", 50, 35} };

	// Shop data
	g.balance = 5;
	g.isEnoughBalance = true;

	// UI flags
	g.playing = true;
	g.usedItem = false;
	g.success = true;
	g.hasEnergy = true;
}

void process_one_action(GameState& g) {

	if (g.scene == Scene::Inventory) {
		inventory_handle_input(g);
		return;
	}
	if (g.scene == Scene::Shop) {
		shop_handle_input(g);
		return;
	} else {
		pet_check_health(g);
		pet_handle_input(g);
	}
}

void pet_handle_input(GameState& g) {

	if (g.playPressed == true) {
		int before = g.boredomNum;
		pet_play(g);
		int after = g.boredomNum;
		g.newStat = before - after;
		g.playPressed = false;

		g.statChanged = true;
		tick(g);
		g.statChanged = false;
	}
	if (g.feedPressed == true) {
		int before = g.hungerNum;
		pet_feed(g);
		int after = g.hungerNum;
		g.newStat = before - after;
		g.feedPressed = false;

		g.statChanged = true;
		tick(g);
		g.statChanged = false;
	}
	if (g.sleepPressed == true) {
		int before = g.energy;
		pet_sleep(g);
		int after = g.energy;
		g.newStat = after - before;
		g.sleepPressed = false;

		g.statChanged = true;
		tick(g);
		g.statChanged = false;
	}


	return;
}

void inventory_handle_input(GameState& g) {

	int choice{};

	std::cout << "Make a choice (0-" << g.inventoryItems.size() << "): ";

	if (!(std::cin >> choice)) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		return;
	}

	if ((choice < 0) || (choice > static_cast<int>(g.inventoryItems.size()))) {
		std::cin.ignore(1000, '\n');
		std::cout << "Wrong input.\n";
		return;
	}

	if (choice == 0) {
		g.scene = Scene::Pet;
		g.usedItem = false;
		inventory_clear_message(g);
	} else {
		use_item(g, choice - 1);
		g.usedItem = true;
		g.scene = Scene::Pet;
		return;
	}

	if (!g.usedItem) inventory_clear_message(g);
}

void shop_handle_input(GameState& g) {

	int choice{};

	std::cout << "Make a choice (0-" << g.shopItems.size() << "): ";

	if (!(std::cin >> choice)) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		return;
	}

	if ((choice < 0) || (choice > static_cast<int>(g.shopItems.size()))) {
		std::cin.ignore(1000, '\n');
		std::cout << "Wrong input.\n";
		return;
	}

	if (choice == 0) {
		g.scene == Scene::Pet;
		transaction_clear_message(g);
	} else {
		buy_item(g, choice - 1);
	}
}

void make_decision(GameState& g, int input) {

	switch (input) {
		case 0:
			g.reaction = "Goodbye!";
			g.success = false;
			g.playing = false;
			break;
		case 1:
			if (g.energy > 0) {
				pet_feed(g);
				g.success = true;
			} else {
				g.success = false;
			}
			break;
		case 2:
			if (g.energy > 0) {
				pet_play(g);
				g.success = true;
			} else {
				g.success = false;
			}
			break;
		case 3:
			pet_sleep(g);
			g.success = true;
			break;
		case 4:
			g.scene = Scene::Inventory;
			g.success = false;
			break;
		case 5:
			g.scene = Scene::Shop;
			g.success = false;
			break;
		default:
			g.success = false;
			break;
	}
}

void tick(GameState& g) {

	pet_get_condition(g);

	g.tickCount++;

	pet_change_health(g, -2);
	pet_change_hunger(g, 5);
	pet_change_boredom(g, 3);
	change_balance(g, TICK_MONEY);
	pet_update_age(g);
	pet_update_warnings(g);

	// Requests
	if (g.birthday)    g.pendingPopups.push_back({"It's my birthday today!", GameEvent::EVENT, TopCenter});
	if (g.isIll)       g.pendingPopups.push_back({"I feel sick...", GameEvent::WARNING, LeftUp});
	if (g.starving)    g.pendingPopups.push_back({"I am starving!", GameEvent::WARNING, LeftCenter});
	if (g.bored)       g.pendingPopups.push_back({"I want to play!", GameEvent::WARNING, LeftBottom});
	if (g.statChanged) g.pendingPopups.push_back({std::to_string(g.newStat), GameEvent::STAT_CHANGE, BottomLeft});

	return;
}

std::vector<Request> drain_pending_popups(GameState& g) {
	
	std::vector<Request> drained;

	drained.swap(g.pendingPopups);

	return drained;
}
#pragma endregion

// Pet ===============================================================================
#pragma region Hunger / Feed
void pet_change_hunger(GameState& g, int delta) {
	
	g.hungerNum += delta;
	pet_clamp_hunger(g);
	pet_translate_hunger(g);
}

void pet_clamp_hunger(GameState& g) {
	
	if (g.hungerNum > MAX_HUNGER) g.hungerNum = MAX_HUNGER;
	if (g.hungerNum < 0) g.hungerNum = 0;
}

void pet_translate_hunger(GameState& g) {
	
	g.hunger = HUNGER[get_inverted_level(g.hungerNum, MAX_HUNGER, 5)];
}

void pet_feed(GameState& g) {
	
	pet_change_hunger(g, -7);
	pet_change_health(g, 1);
	pet_change_energy(g, -1);
	pet_set_reaction(g, "Yummy!");
}
#pragma endregion

#pragma region Boredom / Play
void pet_change_boredom(GameState& g, int delta) {
	
	g.boredomNum += delta;
	pet_clamp_boredom(g);
	pet_translate_boredom(g);
}

void pet_clamp_boredom(GameState& g) {
	
	if (g.boredomNum > MAX_BOREDOM) g.boredomNum = MAX_BOREDOM;
	if (g.boredomNum < 0) g.boredomNum = 0;
}

void pet_translate_boredom(GameState& g) {
	
	g.mood = MOOD[get_inverted_level(g.boredomNum, MAX_BOREDOM, 5)];
}

void pet_play(GameState& g) {

	pet_change_boredom(g, -5);
	pet_change_hunger(g, 3);
	pet_change_energy(g, -1);
	pet_set_reaction(g, "Weee!");
}
#pragma endregion

#pragma region Health
void pet_change_health(GameState& g, int delta) {
	
	g.health += delta;
	pet_clamp_health(g);
	pet_translate_health_to_art(g);
}

void pet_clamp_health(GameState& g) {
	
	if (g.health > MAX_HEALTH) g.health = MAX_HEALTH;
	if (g.health < 0) g.health = 0;
}

void pet_translate_health_to_art(GameState& g) {
	
	g.art = ART[get_level(g.health, MAX_HEALTH, 5)];
}
#pragma endregion

#pragma region Energy / Sleep
void pet_change_energy(GameState& g, int delta) {
	
	g.energy += delta;
	pet_clamp_energy(g);
}

void pet_clamp_energy(GameState& g) {
	
	if (g.energy > MAX_ENERGY) g.energy = MAX_ENERGY;
	if (g.energy < 0) g.energy = 0;
}

void pet_sleep(GameState& g) {

	if (g.tickCount % ENERGY_CHECKPOINT == 0)
		pet_change_energy(g, 3);

	pet_set_reaction(g, "Zzz...");
}
#pragma endregion

#pragma region Check health / Die / Evolve
void pet_check_health(GameState& g) {
	
	if (g.health == 0) {
		pet_die(g);
		g.playing = false;
	}
}

void pet_die(GameState& g) {

	g.isAlive = false;
	g.playing = false;
}

void pet_evolve(GameState& g) {

	g.art = "👾";
	g.reaction = "I have evolved! Thank You!";
}
#pragma endregion

#pragma region Update Warnings / Age
void pet_update_warnings(GameState& g) {
	
	if (g.hungerNum > 80) {
		g.starving = true;
	} else {
		g.starving = false;
	}

	if (g.boredomNum > 60)  {
		g.bored = true;
	} else {
		g.bored = false;
	}
}

void pet_update_age(GameState& g) {

	if ((g.tickCount % AGE_CHECKPOINT == 0) && (g.energy > 0)) {
		g.age++;
		g.birthday = true;
	} else {
		g.birthday = false;
	}
}
#pragma endregion

#pragma region Get ill / condition
void pet_get_ill(GameState& g) {

	g.isIll = true;
	pet_change_health(g, -7);
}

void pet_get_condition(GameState& g) {
	
	int randomNum = get_random_number();

	if (randomNum >= THRESHOLD) {
		g.illnessStart = g.tickCount;
		g.illnessEnd = g.illnessStart + ILLNESS_DUR;
	}

	if (g.tickCount < g.illnessEnd) {
		pet_get_ill(g);
	} else if (g.tickCount == g.illnessEnd) {
		g.reaction = "I have recovered!";
		g.isIll = false;
	}
}
#pragma endregion

#pragma region Set art / reaction
void pet_set_art(GameState& g, const std::string& art) {

	g.art = art;
}

void pet_set_reaction(GameState& g, const std::string& reaction) {

	g.reaction = reaction;
}
#pragma endregion

#pragma region Goal reached
bool pet_goal_reached(GameState& g) {
	
	return g.tickCount >= EVOLVE_TICK;
}
#pragma endregion

// Inventory =========================================================================
#pragma region Inventory
void use_item(GameState& g, int index) {

	if (index < 0 || index >= g.inventoryItems.size()) return;

	Item item = g.inventoryItems[index];

	switch (item.type) {
		case ItemType::MEDICINE: {
			pet_change_health(g, item.value);
			inventory_set_message(g, item.name + " increased health by " + std::to_string(item.value) + " points!");
			} break;
		case ItemType::MOOD: {
			pet_change_boredom(g, -item.value);
			inventory_set_message(g, item.name + " decreased boredom by " + std::to_string(item.value) + " points!");
			} break;
		case ItemType::FOOD: {
			pet_change_hunger(g, -item.value);
			inventory_set_message(g, item.name + " decreased hunger by " + std::to_string(item.value) + " points!");
			} break;
		case ItemType::ENERGY: {
			pet_change_energy(g, item.value);
			inventory_set_message(g, item.name + " increased energy by " + std::to_string(item.value) + " points!");
			} break;
		case ItemType::UNIVERSAL: {
			pet_change_health(g, item.value);
			pet_change_hunger(g, -item.value);
			pet_change_boredom(g, -item.value);
			pet_change_energy(g, item.value);
			inventory_set_message(g, item.name + " improved everything by " + std::to_string(item.value) + " points!");
			} break;
	}

	g.inventoryItems.erase(g.inventoryItems.begin() + index);
}

void inventory_clear_message(GameState& g) {

	g.inventoryMessage.clear();
}

void inventory_set_message(GameState& g, std::string message) {

	g.inventoryMessage = message;
}
#pragma endregion

// Shop ==============================================================================
#pragma region Shop
void buy_item(GameState& g, int index) {

	if (index < 0 || index >= g.shopItems.size()) return;

	Item item = g.shopItems[index];

	if (g.balance >= item.price) {
		change_balance(g, -item.price);
		g.inventoryItems.push_back(g.shopItems[index]);
		g.shopItems.erase(g.shopItems.begin() + index);
		transaction_set_message(g, "You've purchased '" + item.name + "' for $" + std::to_string(item.price));
	} else {
		transaction_set_message(g, "Insufficient balance.");
	}
}

void change_balance(GameState& g, int delta) {

	g.balance += delta;
	clamp_balance(g);
}

void clamp_balance(GameState& g) {

	if (g.balance > MAX_BALANCE) g.balance = MAX_BALANCE;
	if (g.balance < 0) g.balance = 0;
}

void transaction_clear_message(GameState& g) {

	g.transactionMessage.clear();
}

void transaction_set_message(GameState& g, std::string message) {

	g.transactionMessage = message;
}

#pragma endregion