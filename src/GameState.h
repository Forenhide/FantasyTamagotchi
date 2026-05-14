#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Item.h"
#include "PopupNotification.h"
#include <vector>
#include <string>

constexpr int MAX_HEALTH{100};
constexpr int MAX_ENERGY{30};
constexpr int MAX_HUNGER{100};
constexpr int MAX_BOREDOM{75};
constexpr int MAX_BALANCE{100};
constexpr int AGE_CHECKPOINT{30};
constexpr int EVOLVE_TICK{300};
constexpr int THRESHOLD{950};
constexpr int ILLNESS_DUR{5};
constexpr int ENERGY_CHECKPOINT{2};
constexpr int TICK_MONEY{1};

const std::string HUNGER[5]{ "Starving", "Hungry", "Okay", "Well-fed", "Full" };
const std::string MOOD[5]{"Frustrated", "Bored", "Okay", "Good", "Happy"};
const std::string ART[5]{"😭", "😒", "😶", "😊", "😀"};


enum class Scene {

	Pet,
	Inventory,
	Shop
};

struct GameState {

	// Scene
	Scene scene;

	// Pet data
	std::string name;
	std::string hunger;
	std::string mood;
	std::string art;
	std::string reaction;
	std::string warning;
	std::vector<std::string> warnings;
	int health{};
	int energy{};
	int age{};
	int hungerNum{};
	int boredomNum{};
	int illnessStart{};
	int illnessEnd{};
	int tickCount{};
	int newStat{};
	bool isAlive{};
	bool birthday{};
	bool isIll{};
	bool starving{};
	bool bored{};
	bool isPlaying{false};

	// Inventory & Shop
	std::vector<Item> inventoryItems;
	std::vector<Item> shopItems;
	std::string inventoryMessage;

	// Shop data
	int balance{5};
	bool isEnoughBalance{true};
	std::string transactionMessage;

	// UI flags
	bool playing{true};
	bool usedItem{false};
	bool success{true};
	bool hasEnergy{true};
	bool playPressed{false};
	bool feedPressed{false};
	bool sleepPressed{false};
	bool statChanged{false};

	// Popups
	std::vector<Request> pendingPopups; 
};

// Game
void game_init(GameState& g);
void process_one_action(GameState& g);
void pet_handle_input(GameState& g);
void inventory_handle_input(GameState& g);
void shop_handle_input(GameState& g);
void make_decision(GameState& g, int input);
void tick(GameState& g);

// Pet
void pet_feed(GameState& g);
void pet_play(GameState& g);
void pet_sleep(GameState& g);
void pet_check_health(GameState& g);
void pet_die(GameState& g);
void pet_evolve(GameState& g);
void pet_update_warnings(GameState& g);
void pet_update_age(GameState& g);
void pet_change_hunger(GameState& g, int delta);
void pet_change_boredom(GameState& g, int delta);
void pet_change_health(GameState& g, int delta);
void pet_change_energy(GameState& g, int delta);
void pet_clamp_health(GameState& g);
void pet_clamp_hunger(GameState& g);
void pet_clamp_boredom(GameState& g);
void pet_clamp_energy(GameState& g);
void pet_translate_hunger(GameState& g);
void pet_translate_boredom(GameState& g);
void pet_translate_health_to_art(GameState& g);
void pet_get_ill(GameState& g);
void pet_get_condition(GameState& g);
void pet_set_art(GameState& g, const std::string& art);
void pet_set_reaction(GameState& g, const std::string& reaction);
bool pet_goal_reached(GameState& g);

// Inventory
void use_item(GameState& g, int index);
void inventory_clear_message(GameState& g);
void inventory_set_message(GameState& g, std::string message);

// Shop
void buy_item(GameState& g, int index);
void change_balance(GameState& g, int delta);
void clamp_balance(GameState& g);
void transaction_clear_message(GameState& g);
void transaction_set_message(GameState& g, std::string message);

// Requests
std::vector<Request> drain_pending_popups(GameState& g);

#endif