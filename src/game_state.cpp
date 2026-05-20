#include "game_state.h"
#include <iostream>
#include <random>
#include <cmath>
#include <cstdlib>

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

void game_init(GameState& g) {

	// Scene
	g.scene = Scene::Pet;
	
	// Pet
	g.name = "Tony Tony Chopper";
	g.hunger = HUNGER[4];
	g.mood = MOOD[4];
	g.health = 100;
	g.energy = 70;
	g.age = 0;
	g.hungerNum = 20;
	g.boredomNum = 10;
	g.birthday = false;
	g.isIll = false;
	g.sleeping = false;

	g.level = 1;
	g.experience = 0;
	g.nextLevelExp = 100;

	g.illDuration = 15.f;

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

	update_shop(g);

	// Shop data
	g.balance = 25;

	// UI flags
	g.usedItem = false;
	g.boughtItem = false;
	g.playPressed = false;
	g.feedPressed = false;
	g.sleepPressed = false;
}

void pet_handle_input(GameState& g) {

	if (g.playPressed == true) {
		int before = g.boredomNum;
		pet_play(g);
		int after = g.boredomNum;
		g.newStat = before - after;

		g.pendingPopups.push_back({"+" + std::to_string(g.newStat), GameEvent::STAT_CHANGE, BottomLeft});

		g.playPressed = false;
		g.sleeping = false;
	}

	if (g.feedPressed == true) {
		int before = g.hungerNum;
		pet_feed(g);
		int after = g.hungerNum;
		g.newStat = before - after;

		g.pendingPopups.push_back({"+" + std::to_string(g.newStat), GameEvent::STAT_CHANGE, BottomLeft});

		g.feedPressed = false;
		g.sleeping = false;
	}

	if (g.sleepPressed == true) {
		g.sleeping = true;
		g.newStat = 0;
		g.sleepTimer = 0.f;

		g.pendingPopups.push_back({"Sleeping..", GameEvent::EVENT, BottomLeft});

		g.sleepPressed = false;
	}

	return;
}

void tick(GameState& g, float dt) {

    g.totalPlayTime += dt;

    // HUNGER
    g.timeSinceLastHunger += dt;
    if (g.timeSinceLastHunger >= HUNGER_INTERVAL) {
        pet_change_hunger(g, 2);
        g.timeSinceLastHunger = 0.f;
		std::cout << "Hunger changed\n";
    }

    // BOREDOM
    g.timeSinceLastBoredom += dt;
    if (g.timeSinceLastBoredom >= BOREDOM_INTERVAL) {
        pet_change_boredom(g, 2);
        g.timeSinceLastBoredom = 0.f;
		std::cout << "Mood changed\n";
    }
	
	// SLEEP
	if (g.sleeping) {
		g.sleepTimer += dt;

		if (g.sleepTimer >= 1.f) {
			pet_change_energy(g, 5);

			g.newStat += 5;

			g.sleepTimer = 0.f;
		}


		if (g.energy >= MAX_ENERGY) {
			g.sleeping = false;

			g.pendingPopups.push_back({"Fully Rested!", GameEvent::EVENT, BottomLeft});

			g.newStat = 0;
		}
	}

    // ENERGY DRAIN
    g.timeSinceLastEnergy += dt;
    if (g.timeSinceLastEnergy >= ENERGY_INTERVAL) {
        pet_change_energy(g, -2);
        g.timeSinceLastEnergy = 0.f;
		std::cout << "Energy changed\n";
    }

    // AGE
    g.timeSinceLastAge += dt;
    if (g.timeSinceLastAge >= AGE_INTERVAL) {
        g.age++;
        g.birthday = true;
        g.timeSinceLastAge = 0.f;
		std::cout << "Age changed\n";
    } else g.birthday = false;

    // INCOME
    g.timeSinceLastIncome += dt;
    if (g.timeSinceLastIncome >= INCOME_INTERVAL) {
        change_balance(g, 1);
        g.timeSinceLastIncome = 0.f;
    }

    // SHOP REFRESH
    g.timeSinceLastShop += dt;
    if (g.timeSinceLastShop >= SHOP_INTERVAL && g.scene != Scene::ShopTooltip && g.scene != Scene::Shop) {
        update_shop(g);
        g.timeSinceLastShop = 0.f;
		std::cout << "Shop refreshed\n";
    }

    // ILLNESS
	g.timeSinceLastIllness += dt;

	if (g.timeSinceLastIllness >= ILLNESS_CHECK_INTERVAL) {
		pet_get_condition(g);
		g.timeSinceLastIllness = 0.f;
	}

	if (g.isIll) {

		g.illTimer += dt;
		g.illDurationTimer += dt;

		if (g.illTimer >= 1.f) {
			pet_change_health(g, -1);
			g.illTimer = 0.f;
		}

		if (g.illDurationTimer >= g.illDuration) {
			g.isIll = false;

			g.illTimer = 0.f;
			g.illDurationTimer = 0.f;

			g.pendingPopups.push_back({
				"I feel better!",
				GameEvent::EVENT,
				LeftUp
			});
		}
	}
	
	// Derived
    pet_update_warnings(g);
    pet_check_death(g);
    pet_check_evolve(g);
}

void pet_get_condition(GameState& g)
{
    int risk = 5;

    risk += g.hungerNum / 8;
    risk += (100 - g.energy) / 10;

    if (risk > 60) risk = 60;

    int random = rand() % 100;

    if (!g.isIll && random < risk) {
        g.isIll = true;

        g.illTimer = 0.f; 
        g.illDurationTimer = 0.f; 

        g.pendingPopups.push_back({
            "I feel sick..",
            GameEvent::WARNING,
            LeftUp
        });
    }
}

void update_shop(GameState& g) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> randomIndex(0, g.items.size() - 1);

	std::cout << "* Checking g.items size before update: " << g.items.size() << '\n';
	std::cout << "* Checking g.shop size before update: " << g.shop.size() << "\n\n";

	g.shop.clear();
	g.shop.reserve(MAX_ITEM_SLOTS);

	std::cout << "std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<int> randomIndex(0, g.items.size() - 1);\n";

	std::cout << "----Updating Shop----\n";
	for (int i = 0; i < MAX_ITEM_SLOTS; ++i) {
		int index = randomIndex(gen);
		std::cout << "int index = randomIndex(gen);\n";
		std::cout << "Index = " << index << " : " << g.items[index].name << '\n';
		g.shop.push_back(g.items[index]);
	}

	std::cout << "* Checking g.shop size after update: " << g.shop.size() << "\n\n";
}

std::vector<Request> drain_pending_popups(GameState& g) {
	
	std::vector<Request> drained;

	drained.swap(g.pendingPopups);

	return drained;
}

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
	
	pet_change_hunger(g, -18);
	pet_change_energy(g, 2);
	pet_change_exp(g, 5);
}

void pet_change_exp(GameState& g, int delta) {

	g.experience += delta;

	while (g.experience >= g.nextLevelExp) {
		pet_level_up(g);
	}

	if (g.level >= MAX_LEVEL && g.experience > g.nextLevelExp) g.experience = g.nextLevelExp;
}

void pet_level_up(GameState& g) {
	
	if (g.level >= MAX_LEVEL) return;

	g.experience -= g.nextLevelExp;

	g.level++;

	g.nextLevelExp = static_cast<int>(100 + (50 * pow(g.level, 1.35)));

	pet_change_health(g, 25);

	g.pendingPopups.push_back({"Level Up!", GameEvent::EVENT, TopCenter});
}

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

	pet_change_boredom(g, -20);
	pet_change_hunger(g, 5);
	pet_change_energy(g, -8);
	pet_change_exp(g, 7);
}

void pet_change_health(GameState& g, int delta) {
	
	g.health += delta;

	pet_clamp_health(g);
	pet_check_death(g);
}

void pet_clamp_health(GameState& g) {
	
	if (g.health > MAX_HEALTH) g.health = MAX_HEALTH;
	if (g.health < 0) g.health = 0;
}

void pet_change_energy(GameState& g, int delta) {

    g.energy += delta;
    pet_clamp_energy(g);
}

void pet_clamp_energy(GameState& g) {
	
	if (g.energy > MAX_ENERGY) g.energy = MAX_ENERGY;
	if (g.energy < 0) g.energy = 0;
}

void pet_sleep(GameState& g) {

	pet_change_energy(g, 35);
	pet_change_hunger(g, 6);
	pet_change_boredom(g, 4);
	pet_change_health(g, 2);
	pet_change_exp(g, 3);
}

void pet_update_warnings(GameState& g) {
	
	g.starving = g.hungerNum >= 80;
	g.bored = g.boredomNum >= 70;
	g.exhausted = g.energy <= 15;
}

void pet_check_evolve(GameState& g) {
	
	if (g.level >= MAX_LEVEL && g.health > 0) {
		g.scene = Scene::Evolve;
	}
}

// Inventory =========================================================================
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
				pet_change_hunger(g, -item.itemEffects[i].value);
				g.pendingPopups.push_back({"-" + std::to_string(item.itemEffects[i].value) + " hunger", GameEvent::ITEM_USED, BottomCenter});
			} break;
			case ItemEffect::StatType::MOOD: {
				pet_change_boredom(g, -item.itemEffects[i].value);
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

// Shop ==============================================================================
void buy_item(GameState& g, int index) {

	if (index < 0 || index >= g.shop.size()) return;

	Item item = g.shop[index];

	if (g.balance >= item.price) {
		if (g.inventory.size() >= MAX_ITEM_SLOTS) {
			g.boughtItem = false;
			g.pendingPopups.push_back({"Not enough space!", GameEvent::ITEM_BOUGHT, ShopBottomLeft});
		} else {
			change_balance(g, -item.price);
			g.inventory.push_back(g.shop[index]);
			g.shop.erase(g.shop.begin() + index);
			g.boughtItem = true;
			g.pendingPopups.push_back({"+ " + item.name, GameEvent::ITEM_BOUGHT, ShopBottomLeft});
			std::cout << "BOUGTH item name: " << item.name << '\n';
			std::cout << "* BOUGHT ITEM index " << index << '\n';
		}
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

void pet_check_death(GameState& g) {
	
	if (g.health <= 0) g.scene = Scene::Death;
}