#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "item.h"
#include "popup_notification.h"

#include <vector>
#include <string>


constexpr int MAX_HEALTH{100};
constexpr int MAX_ENERGY{100};
constexpr int MAX_HUNGER{100};
constexpr int MAX_BOREDOM{100};
constexpr int MAX_BALANCE{9999};
constexpr int MAX_ITEM_SLOTS{24};
constexpr int MAX_LEVEL{20};

constexpr float HUNGER_INTERVAL{20.f};
constexpr float BOREDOM_INTERVAL{15.f};
constexpr float ENERGY_INTERVAL{35.f};
constexpr float INCOME_INTERVAL{10.f};
constexpr float AGE_INTERVAL{60.f};
constexpr float SHOP_INTERVAL{30.f};
constexpr float ILLNESS_CHECK_INTERVAL{5.f};

const std::string HUNGER[5]{ "Starving", "Hungry", "Okay", "Well-fed", "Full" };
const std::string MOOD[5]{"Frustrated", "Bored", "Okay", "Good", "Happy"};

enum class Scene {

	Menu,
	Pet,
	Inventory,
	InventoryTooltip,
	Shop,
	ShopTooltip,
	Death,
	Evolve,
};

struct GameState {

	// Scene
	Scene scene;

	// Pet data
	std::string hunger;
	std::string mood;
	std::string warning;
	std::vector<std::string> warnings;

	int health{};
	int energy{};
	int age{};

	int hungerNum{};
	int boredomNum{};

	int level;
	int experience;
	int nextLevelExp;

	bool birthday{};
	bool isIll{};
	bool starving{};
	bool bored{};
	bool exhausted{};
	bool sleeping{};
	
	// Timers
	float totalPlayTime{};
	float sleepTimer{};
	float illTimer{};
	float illDurationTimer{};
	float illDuration{};

	float timeSinceLastHunger{};
	float timeSinceLastBoredom{};
	float timeSinceLastEnergy{};
	float timeSinceLastAge{};
	float timeSinceLastShop{};
	float timeSinceLastIncome{};
	float timeSinceLastIllness{};

	// Inventory & Shop
	std::vector<Item> items;
	std::vector<Item> inventory;
	std::vector<Item> shop;

	std::string inventoryMessage;

	// Shop data
	int balance{5};

	// UI flags
	bool usedItem{};		// DELETE
	bool boughtItem{};		// DELETE
	bool playPressed{};
	bool feedPressed{};
	bool sleepPressed{};

	// Popups
	std::vector<Request> pendingPopups; 
};

// Game
void game_init(GameState& g);
//void tick(GameState& g);
void tick(GameState& g, float dt);


// Pet
void pet_handle_input(GameState& g);

void pet_feed(GameState& g);
void pet_play(GameState& g);

void pet_change_health(GameState& g, int delta);
void pet_change_hunger(GameState& g, int delta);
void pet_change_boredom(GameState& g, int delta);
void pet_change_energy(GameState& g, int delta);

void pet_clamp_health(GameState& g);
void pet_clamp_hunger(GameState& g);
void pet_clamp_boredom(GameState& g);
void pet_clamp_energy(GameState& g);

void pet_translate_hunger(GameState& g);
void pet_translate_boredom(GameState& g);

void pet_check_death(GameState& g);
void pet_check_evolve(GameState& g);

void pet_update_warnings(GameState& g);

void pet_change_exp(GameState& g, int delta);

void pet_get_condition(GameState& g);

// Inventory
void use_item(GameState& g, int index);

// Shop
void update_shop(GameState& g);

void buy_item(GameState& g, int index);

void change_balance(GameState& g, int delta);
void clamp_balance(GameState& g);

// Requests
std::vector<Request> drain_pending_popups(GameState& g);

#endif