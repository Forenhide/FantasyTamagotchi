#include "game_state.h"
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
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> randomNum(0, 1000);

	return randomNum(gen);
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
	g.birthday = false;
	g.isIll = false;

	// Items
	g.items = { {"Healing Potion", 10, 0, { {5, ItemEffect::StatType::HEALTH} }},
				{"Joy Potion", 10, 1, { {7, ItemEffect::StatType::MOOD} }},
				{"Energy Potion", 10, 2, { {9, ItemEffect::StatType::ENERGY} }},
				{"Universal Potion", 50, 3, { {35, ItemEffect::StatType::HEALTH}, {50, ItemEffect::StatType::HUNGER}, {50, ItemEffect::StatType::MOOD}, {50, ItemEffect::StatType::ENERGY} }},
				{"Mochi", 12, 4, { {5, ItemEffect::StatType::HUNGER} }},
				{"Pineapple", 35, 5, { {25, ItemEffect::StatType::HUNGER} }},
				{"Teddy Bear", 7, 6, { {3, ItemEffect::StatType::MOOD} }},
				{"Tetris", 11, 7, { {5, ItemEffect::StatType::MOOD} }},
				{"Tiramisu", 20, 8, {{ 14, ItemEffect::StatType::HUNGER} }},
				{"Yo-Yo", 5, 9, { {2, ItemEffect::StatType::MOOD} }} };

	g.inventory = { g.items[0], g.items[1], g.items[2], g.items[3] };

	populate_shop_at_start(g);

	// Shop data
	g.balance = 5;
	g.isEnoughBalance = true;

	// UI flags
	g.playing = true;
	g.usedItem = false;
	g.success = true;
	g.hasEnergy = true;
}

void populate_shop_at_start(GameState& g) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> randomIndex(0, g.items.size() - 1);

	g.shop.clear();
	g.shop.reserve(MAX_ITEM_SLOTS);

	for (int i = 0; i < MAX_ITEM_SLOTS; ++i) {
		int index = randomIndex(gen);
		g.shop.push_back(g.items[index]);
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

void tick(GameState& g) {

	pet_check_death(g);
	pet_check_evolve(g);

	if (g.scene == Scene::Death || g.scene == Scene::Evolve) return;

	pet_get_condition(g);

	g.tickCount++;

	pet_change_hunger(g, -5);
	pet_change_boredom(g, -3);
	change_balance(g, TICK_MONEY);
	pet_update_age(g);
	pet_update_warnings(g);

	// Requests
	if (g.birthday)    g.pendingPopups.push_back({"It's my birthday today!", GameEvent::EVENT, TopCenter});
	if (g.isIll)       g.pendingPopups.push_back({"I feel sick...", GameEvent::WARNING, LeftUp});
	if (g.starving)    g.pendingPopups.push_back({"I am starving!", GameEvent::WARNING, LeftCenter});
	if (g.bored)       g.pendingPopups.push_back({"I want to play!", GameEvent::WARNING, LeftBottom});
	if (g.statChanged) g.pendingPopups.push_back({std::to_string(g.newStat), GameEvent::STAT_CHANGE, BottomLeft});
	
	g.statChanged = false;

	update_shop(g);

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

    g.hungerNum -= delta;
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
	
	pet_change_hunger(g, 7);
	pet_change_health(g, 1);
	pet_change_energy(g, -1);
}
#pragma endregion

#pragma region Boredom / Play
void pet_change_boredom(GameState& g, int delta) {

    g.boredomNum -= delta;
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

	pet_change_boredom(g, 5);
	pet_change_hunger(g, -3);
	pet_change_energy(g, -1);
}
#pragma endregion

#pragma region Health
void pet_change_health(GameState& g, int delta) {
	
	g.health += delta;

	pet_clamp_health(g);
	pet_check_death(g);
}

void pet_clamp_health(GameState& g) {
	
	if (g.health > MAX_HEALTH) g.health = MAX_HEALTH;
	if (g.health < 0) g.health = 0;
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

	pet_change_energy(g, 1);
	pet_change_health(g, -1);
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
	pet_change_health(g, -2);
}

void pet_get_condition(GameState& g) {
	
	int randomNum = get_random_number();

	if (randomNum >= THRESHOLD) {
		g.illnessStart = g.tickCount;
		g.illnessEnd = g.illnessStart + ILLNESS_DUR;
	}

	if (g.tickCount < g.illnessEnd) pet_get_ill(g);
	else if (g.tickCount == g.illnessEnd) g.isIll = false;

	if (g.hungerNum >= MAX_HUNGER) pet_change_health(g, -5);
	if (g.boredomNum >= MAX_BOREDOM) pet_change_health(g, -5);
}
#pragma endregion

#pragma region Goal reached
void pet_check_evolve(GameState& g) {
	
	if (g.tickCount >= EVOLVE_TICK) g.scene = Scene::Evolve;
}
#pragma endregion

// Inventory =========================================================================
#pragma region Inventory
void use_item(GameState& g, int index) {

	if (index < 0 || index >= static_cast<int>(g.inventory.size())) return;

	Item& item = g.inventory[index];
	
	for (int i = 0; i < item.itemEffects.size(); ++i) {
		switch (item.itemEffects[i].type) {
			case ItemEffect::StatType::HEALTH: {
				pet_change_health(g, item.itemEffects[i].value);
				g.pendingPopups.push_back({"+" + std::to_string(item.itemEffects[i].value) + " health", GameEvent::ITEM_USED, BottomCenter});
			} break;
			case ItemEffect::StatType::HUNGER: {
				pet_change_hunger(g, item.itemEffects[i].value);
				g.pendingPopups.push_back({"-" + std::to_string(item.itemEffects[i].value) + " hunger", GameEvent::ITEM_USED, BottomCenter});
			} break;
			case ItemEffect::StatType::MOOD: {
				pet_change_boredom(g, item.itemEffects[i].value);
				g.pendingPopups.push_back({"-" + std::to_string(item.itemEffects[i].value) + " boredom", GameEvent::ITEM_USED, BottomCenter});
			} break;
			case ItemEffect::StatType::ENERGY: {
				pet_change_energy(g, item.itemEffects[i].value);
				g.pendingPopups.push_back({"+" + std::to_string(item.itemEffects[i].value) + " energy", GameEvent::ITEM_USED, BottomCenter});
			} break;
			default: break;
		}
	} 

	g.usedItem = true;

	g.inventory.erase(g.inventory.begin() + index);
}
#pragma endregion

// Shop ==============================================================================
#pragma region Shop
void buy_item(GameState& g, int index) {

	if (index < 0 || index >= g.shop.size()) return;

	Item& item = g.shop[index];

	if (g.balance >= item.price) {
		change_balance(g, -item.price);
		g.inventory.push_back(g.shop[index]);
		g.shop.erase(g.shop.begin() + index);
		g.boughtItem = true;
		g.pendingPopups.push_back({"+ " + item.name, GameEvent::ITEM_BOUGHT, ShopBottomLeft});
	} else {
		g.boughtItem = false;
		g.pendingPopups.push_back({"Insufficient Balance!", GameEvent::ITEM_BOUGHT, ShopBottomLeft});
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

void update_shop(GameState& g) {
	
	if (g.tickCount > 0 && g.tickCount % SHOP_UPDATE_TICK == 0) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> randomIndex(0, g.items.size() - 1);

		g.shop.clear();

		for (int i = 0; i < MAX_ITEM_SLOTS; ++i) {
			int index = randomIndex(gen);
			g.shop.push_back(g.items[index]);
		}	
	}
}
#pragma endregion

void pet_check_death(GameState& g) {
	
	if (g.health <= 0) g.scene = Scene::Death;
}