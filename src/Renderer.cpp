#include "renderer.h"
#include <windows.h>
#include <iostream>
#include <algorithm>


Renderer::Renderer(GameState& gameState) : 

	gameState(gameState),
	petBgSprite(petBgTexture),
	inventoryBgSprite(inventoryBgTexture),
	shopBgSprite(shopBgTexture),
	petIdleSprite(petIdleTexture),
	petPlaySprite(petPlayTexture),
	petHurtSprite(petHurtTexture),
	outerShadowSprite(outerShadowTexture), 
	innerShadowSprite(innerShadowTexture), 
	playBtnSprite(playBtnTexture),
	feedBtnSprite(feedBtnTexture),
	sleepBtnSprite(sleepBtnTexture),
	inventoryBtnSprite(inventoryBtnTexture),
	shopBtnSprite(shopBtnTexture),
	inventoryBackToBtnSprite(backBtnTexture),
	shopBackToBtnSprite(backBtnTexture),
	petBackToBtnSprite(backBtnTexture),
	useBtnSprite(useBtnTexture),
	buyBtnSprite(buyBtnTexture),
	nameRecSprite(statRecTexture),
	healthRecSprite(statRecTexture),
	hungerRecSprite(statRecTexture),
	moodRecSprite(statRecTexture),
	energyRecSprite(smallStatRecTexture),
	ageRecSprite(smallStatRecTexture),
	itemSlotRecSprite(itemSlotRecTexture),
	tooltipRecSprite(tooltipRecTexture),

	healingPotionSprite(healingPotionTexture),
	joyPotionSprite(joyPotionTexture),
	energyPotionSprite(energyPotionTexture),
	universalPotionSprite(universalPotionTexture),
	mochiSprite(mochiTexture),
	pineappleSprite(pineappleTexture),
	teddyBearSprite(teddyBearTexture),
	tetrisSprite(tetrisTexture),
	tiramisuSprite(tiramisuTexture),
	yoYoSprite(yoYoTexture),

	title1(font, "FANTASY", 48),
	title2(font, "TAMAGOTCHI", 48),
	subtitle(font, "* Kitty Edition *", 20),
	inventoryTitle(font, "INVENTORY", 48),
	shopTitle(font, "SHOP", 48),
	nameTitle(font),
	nameStat(font),
	healthTitle(font),
	healthStat(font),
	hungerTitle(font),
	hungerStat(font),
	moodTitle(font),
	moodStat(font),
	energyTitle(font),
	energyStat(font),
	ageTitle(font),
	ageStat(font),

	itemName(font)
{}

bool Renderer::init(void) {
	
	window.create(sf::VideoMode({600, 1024}), "Fantasy Tamagotchi (Kitty Edition)", sf::Style::Close | sf::Style::Titlebar);

	load_resources();
	setup_sprites();
	setup_button_sprites();
	setup_buttons();
	setup_titles();
	setup_stats();
	setup_pet_idle_animation();
	setup_pet_play_animation();
	apply_scene_layout(gameState.scene);

	// Inventory
	setup_items_position();
	setup_slot_sprites();
	setup_tooltip_window();

	if(!soundMgr.init()) throw std::runtime_error("sound manager init");

	music.play();
	music.setVolume(0.f);
	music.setLooping(true);

	return true;
}

bool Renderer::is_open(void) const {
	
	return window.isOpen();
}

void Renderer::load_resources(void) {
	
	// Backgrounds
	if (!petBgTexture.loadFromFile("assets/sprites/background.jpg")) throw std::runtime_error("bg");
	if (!inventoryBgTexture.loadFromFile("assets/sprites/inventoryBg.jpg")) throw std::runtime_error("bg");
	if (!shopBgTexture.loadFromFile("assets/sprites/shopBg.jpg")) throw std::runtime_error("bg");

	// Pet
	if (!petIdleTexture.loadFromFile("assets/sprites/pet/IDLE.png")) throw std::runtime_error("pet");
	if (!petPlayTexture.loadFromFile("assets/sprites/pet/PLAY.png")) throw std::runtime_error("pet");
	if (!petHurtTexture.loadFromFile("assets/sprites/pet/HURT.png")) throw std::runtime_error("pet");
	if (!outerShadowTexture.loadFromFile("assets/sprites/pet/shadow1.png")) throw std::runtime_error("outer shadow");
	if (!innerShadowTexture.loadFromFile("assets/sprites/pet/shadow2.png")) throw std::runtime_error("inner shadow");

	// Recs
	if (!statRecTexture.loadFromFile("assets/sprites/recs/rec.png")) throw std::runtime_error("stat rec");
	if (!smallStatRecTexture.loadFromFile("assets/sprites/recs/recSmall.png")) throw std::runtime_error("small stat rec");
	if (!itemSlotRecTexture.loadFromFile("assets/sprites/recs/itemSlotRec.png")) throw std::runtime_error("stat rec");
	if (!tooltipRecTexture.loadFromFile("assets/sprites/recs/tooltipRec.png")) throw std::runtime_error("stat rec");

	// Buttons
	if (!playBtnTexture.loadFromFile("assets/sprites/buttons/playBtn.png")) throw std::runtime_error("play btn");
	if (!feedBtnTexture.loadFromFile("assets/sprites/buttons/feedBtn.png")) throw std::runtime_error("feed btn");
	if (!sleepBtnTexture.loadFromFile("assets/sprites/buttons/sleepBtn.png")) throw std::runtime_error("sleep btn");
	if (!inventoryBtnTexture.loadFromFile("assets/sprites/buttons/inventoryBtn.png")) throw std::runtime_error("inventory btn");
	if (!shopBtnTexture.loadFromFile("assets/sprites/buttons/shopBtn.png")) throw std::runtime_error("shop btn");
	if (!backBtnTexture.loadFromFile("assets/sprites/buttons/backBtn.png")) throw std::runtime_error("back btn");
	if (!useBtnTexture.loadFromFile("assets/sprites/buttons/useBtn.png")) throw std::runtime_error("use btn");
	if (!buyBtnTexture.loadFromFile("assets/sprites/buttons/buyBtn.png")) throw std::runtime_error("buybtn");

	// Items
	if (!healingPotionTexture.loadFromFile("assets/sprites/items/healingPotion.png")) throw std::runtime_error("shop btn");
	if (!joyPotionTexture.loadFromFile("assets/sprites/items/joyPotion.png")) throw std::runtime_error("shop btn");
	if (!energyPotionTexture.loadFromFile("assets/sprites/items/energyPotion.png")) throw std::runtime_error("shop btn");
	if (!universalPotionTexture.loadFromFile("assets/sprites/items/universalPotion.png")) throw std::runtime_error("shop btn");
	if (!mochiTexture.loadFromFile("assets/sprites/items/mochi.png")) throw std::runtime_error("shop btn");
	if (!pineappleTexture.loadFromFile("assets/sprites/items/pineapple.png")) throw std::runtime_error("shop btn");
	if (!teddyBearTexture.loadFromFile("assets/sprites/items/teddyBear.png")) throw std::runtime_error("shop btn");
	if (!tetrisTexture.loadFromFile("assets/sprites/items/tetris.png")) throw std::runtime_error("shop btn");
	if (!tiramisuTexture.loadFromFile("assets/sprites/items/tiramisu.png")) throw std::runtime_error("shop btn");
	if (!yoYoTexture.loadFromFile("assets/sprites/items/yo-yo.png")) throw std::runtime_error("shop btn");

	// Other
	if (!font.openFromFile("assets/fonts/PressStart2P-Regular.ttf")) throw std::runtime_error("font");
	if (!music.openFromFile("assets/sounds/Music.mp3")) throw std::runtime_error("music");
}

void Renderer::setup_sprites(void) {
	
	// Set textures
	petBgSprite.setTexture(petBgTexture, true);
	inventoryBgSprite.setTexture(inventoryBgTexture, true);
	shopBgSprite.setTexture(shopBgTexture, true);

	petIdleSprite.setTexture(petIdleTexture, true);
	petPlaySprite.setTexture(petPlayTexture, true);
	petHurtSprite.setTexture(petHurtTexture, true);
	outerShadowSprite.setTexture(outerShadowTexture, true);
	innerShadowSprite.setTexture(innerShadowTexture, true);

	nameRecSprite.setTexture(statRecTexture, true);
	healthRecSprite.setTexture(statRecTexture, true);
	hungerRecSprite.setTexture(statRecTexture, true);
	moodRecSprite.setTexture(statRecTexture, true);
	energyRecSprite.setTexture(smallStatRecTexture, true);
	ageRecSprite.setTexture(smallStatRecTexture, true);
	itemSlotRecSprite.setTexture(itemSlotRecTexture, true);
	tooltipRecSprite.setTexture(tooltipRecTexture, true);

	playBtnSprite.setTexture(playBtnTexture, true);
	feedBtnSprite.setTexture(feedBtnTexture, true);
	sleepBtnSprite.setTexture(sleepBtnTexture, true);
	inventoryBtnSprite.setTexture(inventoryBtnTexture, true);
	shopBtnSprite.setTexture(shopBtnTexture, true);
	inventoryBackToBtnSprite.setTexture(backBtnTexture, true);
	shopBackToBtnSprite.setTexture(backBtnTexture, true);
	petBackToBtnSprite.setTexture(backBtnTexture, true);
	useBtnSprite.setTexture(useBtnTexture, true);
	buyBtnSprite.setTexture(buyBtnTexture, true);

	healingPotionSprite.setTexture(healingPotionTexture, true);
	joyPotionSprite.setTexture(joyPotionTexture, true);
	energyPotionSprite.setTexture(energyPotionTexture, true);
	universalPotionSprite.setTexture(universalPotionTexture, true);
	mochiSprite.setTexture(mochiTexture, true);
	pineappleSprite.setTexture(pineappleTexture, true);
	teddyBearSprite.setTexture(teddyBearTexture, true);
	tetrisSprite.setTexture(tetrisTexture, true);
	tiramisuSprite.setTexture(tiramisuTexture, true);
	yoYoSprite.setTexture(yoYoTexture, true);

	itemSprites.push_back(healingPotionSprite);
	itemSprites.push_back(joyPotionSprite);
	itemSprites.push_back(energyPotionSprite);
	itemSprites.push_back(universalPotionSprite);
	itemSprites.push_back(mochiSprite);
	itemSprites.push_back(pineappleSprite);
	itemSprites.push_back(teddyBearSprite);
	itemSprites.push_back(tetrisSprite);
	itemSprites.push_back(tiramisuSprite);
	itemSprites.push_back(yoYoSprite);

	// Adjust scale
	petIdleSprite.setScale({15.f, 15.f});
	petPlaySprite.setScale({15.f, 15.f});
	petHurtSprite.setScale({15.f, 15.f});

	// Adjust positions
	petIdleSprite.setPosition({-300.f, 110.f});
	petPlaySprite.setPosition({-300.f, 110.f});
	petHurtSprite.setPosition({-300.f, 110.f});
	outerShadowSprite.setPosition({133.f, 780.f});
	innerShadowSprite.setPosition({182.f, 795.f});
}

void Renderer::setup_pet_idle_animation(void) {
	
	idleFrames.push_back(sf::IntRect({0, 0}, {80, 64}));
	idleFrames.push_back(sf::IntRect({80, 0}, {80, 64}));
	idleFrames.push_back(sf::IntRect({160, 0}, {80, 64}));
	idleFrames.push_back(sf::IntRect({240, 0}, {80, 64}));
	idleFrames.push_back(sf::IntRect({320, 0}, {80, 64}));
	idleFrames.push_back(sf::IntRect({400, 0}, {80, 64}));
	idleFrames.push_back(sf::IntRect({480, 0}, {80, 64}));
	idleFrames.push_back(sf::IntRect({560, 0}, {80, 64}));

	petIdleSprite.setTexture(petIdleTexture, true);
	petIdleSprite.setTextureRect(idleFrames[0]);
}

void Renderer::update_pet_idle_animation(void) {
	
	static sf::Clock clock;

	if (clock.getElapsedTime().asSeconds() >= frameDuration) {
		currentFrame = (currentFrame + 1) % idleFrames.size();
		petIdleSprite.setTextureRect(idleFrames[currentFrame]);
		clock.restart();
	}
}

void Renderer::setup_pet_play_animation(void) {
	
	playFrames.push_back(sf::IntRect({0, 0}, {80, 64}));
	playFrames.push_back(sf::IntRect({80, 0}, {80, 64}));
	playFrames.push_back(sf::IntRect({160, 0}, {80, 64}));
	playFrames.push_back(sf::IntRect({240, 0}, {80, 64}));
	playFrames.push_back(sf::IntRect({320, 0}, {80, 64}));
	playFrames.push_back(sf::IntRect({400, 0}, {80, 64}));
	playFrames.push_back(sf::IntRect({480, 0}, {80, 64}));
	playFrames.push_back(sf::IntRect({560, 0}, {80, 64}));

	petPlaySprite.setTexture(petPlayTexture, true);
	petPlaySprite.setTextureRect(playFrames[0]);
}

void Renderer::update_pet_play_animation(void) {
	
	static sf::Clock clock;

	if (clock.getElapsedTime().asSeconds() >= frameDuration) {
		currentFrame = (currentFrame + 1) % playFrames.size();
		petPlaySprite.setTextureRect(playFrames[currentFrame]);
		clock.restart();
	}
}

void Renderer::setup_stats(void) {

	setup_stats_recs(nameRecSprite, nameTitle, nameStat, "Name", gameState.name, 17.5f, 157.5f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {115, 192, 255});
	setup_stats_recs(healthRecSprite, healthTitle, healthStat, "Health", std::to_string(gameState.health), 17.5f, 246.f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {92, 199, 108});
	setup_stats_recs(hungerRecSprite, hungerTitle, hungerStat, "Hunger", gameState.hunger, 305.5f, 157.5f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {92, 199, 108});
	setup_stats_recs(moodRecSprite, moodTitle, moodStat, "Mood", gameState.mood, 305.5f, 246.5f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {199, 92, 92});
	setup_stats_recs(energyRecSprite, energyTitle, energyStat, "Energy", std::to_string(gameState.energy), 17.5f, 811.5f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {92, 199, 108});
	setup_stats_recs(ageRecSprite, ageTitle, ageStat, "Age", std::to_string(gameState.age), 437.5f, 811.5f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {115, 192, 255});
}

void Renderer::setup_stats_recs(sf::Sprite& rec, sf::Text& titleText, sf::Text& statText, const std::string& title, const std::string& stat, float x, float y, int fontSizeTitle, int fontSizeStat, const sf::Color& fontColorTitle, const sf::Color& fontColorStat, const sf::Color& titleOutlineColor, const sf::Color& statOutlineColor) {

	// Rectangle position
	rec.setPosition({x, y});
	float width = rec.getGlobalBounds().size.x;
	float height = rec.getGlobalBounds().size.y;

	// Text
	titleText.setString(title);
	titleText.setCharacterSize(fontSizeTitle);
	titleText.setFillColor(fontColorTitle);
	titleText.setOutlineColor(titleOutlineColor);
	titleText.setOutlineThickness(1.f);
	statText.setString(stat);
	statText.setCharacterSize(fontSizeStat);
	statText.setFillColor(fontColorStat);
	statText.setOutlineColor(statOutlineColor);
	statText.setOutlineThickness(1.f);

	// Center text
	sf::FloatRect title_bounds = titleText.getLocalBounds();
	sf::FloatRect stat_bounds = statText.getLocalBounds();
	titleText.setOrigin({
		title_bounds.position.x + title_bounds.size.x / 2.f,
		title_bounds.position.y + title_bounds.size.y / 2.f + 16.f
	});
	statText.setOrigin({
		stat_bounds.position.x + stat_bounds.size.x / 2.f,
		stat_bounds.position.y + stat_bounds.size.y / 2.f - 16.f
	});

	titleText.setPosition({x + width / 2.f, y + height / 2.f});
	statText.setPosition({x + width / 2.f, y + height / 2.f});
}

void Renderer::update_stats(void) {
	
	healthStat.setString(std::to_string(gameState.health));
	hungerStat.setString(gameState.hunger);
	moodStat.setString(gameState.mood);
	energyStat.setString(std::to_string(gameState.energy));
	ageStat.setString(std::to_string(gameState.age));

	// Center stats
	sf::FloatRect healthBounds = healthStat.getLocalBounds();
	sf::FloatRect hungerBounds = hungerStat.getLocalBounds();
	sf::FloatRect moodBounds = moodStat.getLocalBounds();
	sf::FloatRect energyBounds = energyStat.getLocalBounds();
	sf::FloatRect ageBounds = ageStat.getLocalBounds();

	healthStat.setOrigin({
		healthBounds.position.x + healthBounds.size.x / 2.f,
		healthBounds.position.y + healthBounds.size.y / 2.f - 16.f
	});
	hungerStat.setOrigin({
		hungerBounds.position.x + hungerBounds.size.x / 2.f,
		hungerBounds.position.y + hungerBounds.size.y / 2.f - 16.f
	});
	moodStat.setOrigin({
		moodBounds.position.x + moodBounds.size.x / 2.f,
		moodBounds.position.y + moodBounds.size.y / 2.f - 16.f
	});
	energyStat.setOrigin({
		energyBounds.position.x + energyBounds.size.x / 2.f,
		energyBounds.position.y + energyBounds.size.y / 2.f - 16.f
	});
	ageStat.setOrigin({
		ageBounds.position.x + ageBounds.size.x / 2.f,
		ageBounds.position.y + ageBounds.size.y / 2.f - 16.f
	});
}

void Renderer::setup_titles(void) {
	
	// Pet
	title1.setFillColor({230, 156, 105});
	title1.setOutlineColor({255, 255, 255});
	title1.setOutlineThickness(1.f);
	title1.setPosition({132.f, 16.f});

	title2.setFillColor({230, 156, 105});
	title2.setOutlineColor({255, 255, 255});
	title2.setOutlineThickness(1.f);
	title2.setPosition({60.f, 68.f});

	subtitle.setFillColor({0, 152, 220});
	subtitle.setOutlineColor({255, 255, 255});
	subtitle.setOutlineThickness(1.f);
	subtitle.setPosition({130.f, 120.f});

	// Inventory
	inventoryTitle.setFillColor({52, 168, 83});
	inventoryTitle.setOutlineThickness(1.f);
	inventoryTitle.setOutlineColor({255, 255, 255});
	inventoryTitle.setPosition({204.f, 16.f});

	// Shop
	shopTitle.setFillColor({222, 145, 77});
	shopTitle.setOutlineThickness(1.f);
	shopTitle.setOutlineColor({255, 255, 255});
	shopTitle.setPosition({204.f, 16.f});
}

void Renderer::apply_scene_layout(const Scene& scene) {
	
	auto& v = buttonsSystem.visuals;

	v[static_cast<size_t>(ButtonType::PlayButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::FeedButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::SleepButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::InventoryButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::ShopButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::PetBackButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::InventoryBackButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::ShopBackButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::UseButton)].targetScale = 0.f;
	v[static_cast<size_t>(ButtonType::BuyButton)].targetScale = 0.f;

	switch (scene) {
		case Scene::Pet: {
			v[static_cast<size_t>(ButtonType::PlayButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::PlayButton)].targetPosition = {24.f, 908.f};

			v[static_cast<size_t>(ButtonType::FeedButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::FeedButton)].targetPosition = {210.f, 908.f};

			v[static_cast<size_t>(ButtonType::SleepButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::SleepButton)].targetPosition = {397.f, 908.f};

			v[static_cast<size_t>(ButtonType::InventoryButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::InventoryButton)].targetPosition = {24.f, 962.f};

			v[static_cast<size_t>(ButtonType::ShopButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::ShopButton)].targetPosition = {360.f, 962.f};
		} break;
		case Scene::Inventory: {
			v[static_cast<size_t>(ButtonType::PetBackButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::PetBackButton)].targetPosition = {397.f, 962.f};
		} break;
		case Scene::InventoryTooltip: {
			v[static_cast<size_t>(ButtonType::InventoryBackButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::InventoryBackButton)].targetPosition = {78.f, 668.f};
			v[static_cast<size_t>(ButtonType::InventoryBackButton)].currentPosition = {-100.f, 668.f};

			v[static_cast<size_t>(ButtonType::UseButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::UseButton)].targetPosition = {344.f, 668.f};
			v[static_cast<size_t>(ButtonType::UseButton)].currentPosition = {700.f, 668.f};
		} break;
		case Scene::Shop: {
			v[static_cast<size_t>(ButtonType::PetBackButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::PetBackButton)].targetPosition = {397.f, 962.f};
		} break;
		case Scene::ShopTooltip: {
			v[static_cast<size_t>(ButtonType::ShopBackButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::ShopBackButton)].targetPosition = {78.f, 668.f};

			v[static_cast<size_t>(ButtonType::BuyButton)].targetScale = 1.f;
			v[static_cast<size_t>(ButtonType::BuyButton)].targetPosition = {344.f, 668.f};
		} break;
		default: break;
	}

	for (auto& v : buttonsSystem.visuals) {
		v.hoverT = 0.f;
	}
}

void Renderer::setup_button_sprites(void) {
	
	playBtnSprite.setTexture(playBtnTexture, true);
	feedBtnSprite.setTexture(feedBtnTexture, true);
	sleepBtnSprite.setTexture(sleepBtnTexture, true);
	inventoryBtnSprite.setTexture(inventoryBtnTexture, true);
	shopBtnSprite.setTexture(shopBtnTexture, true);
	petBackToBtnSprite.setTexture(backBtnTexture, true);
	useBtnSprite.setTexture(useBtnTexture, true);
	buyBtnSprite.setTexture(buyBtnTexture, true);
	inventoryBackToBtnSprite.setTexture(backBtnTexture, true);
	shopBackToBtnSprite.setTexture(backBtnTexture, true);

	buttonSprites.push_back(playBtnSprite);
	buttonSprites.push_back(feedBtnSprite);
	buttonSprites.push_back(sleepBtnSprite);
	buttonSprites.push_back(inventoryBtnSprite);
	buttonSprites.push_back(shopBtnSprite);
	buttonSprites.push_back(inventoryBackToBtnSprite);
	buttonSprites.push_back(shopBackToBtnSprite);
	buttonSprites.push_back(petBackToBtnSprite);
	buttonSprites.push_back(useBtnSprite);
	buttonSprites.push_back(buyBtnSprite);
}

void Renderer::setup_buttons(void) {
	
	Button playBtn;
	playBtn.type = ButtonType::PlayButton;
	playBtn.action = ButtonAction::PetPlay;

	Button feedBtn;
	feedBtn.type = ButtonType::FeedButton;
	feedBtn.action = ButtonAction::PetFeed;

	Button sleepBtn;
	sleepBtn.type = ButtonType::SleepButton;
	sleepBtn.action = ButtonAction::PetSleep;

	Button inventoryBtn;
	inventoryBtn.type = ButtonType::InventoryButton;
	inventoryBtn.action = ButtonAction::OpenInventory;

	Button shopBtn;
	shopBtn.type = ButtonType::ShopButton;
	shopBtn.action = ButtonAction::OpenShop;

	Button inventoryBackToBtn;
	inventoryBackToBtn.type = ButtonType::InventoryBackButton;
	inventoryBackToBtn.action = ButtonAction::BackToInventory;

	Button shopBackToBtn;
	shopBackToBtn.type = ButtonType::ShopBackButton;
	shopBackToBtn.action = ButtonAction::BackToShop;

	Button petBackToBtn;
	petBackToBtn.type = ButtonType::PetBackButton;
	petBackToBtn.action = ButtonAction::BackToPet;

	Button useBtn;
	useBtn.type = ButtonType::UseButton;
	useBtn.action = ButtonAction::UseItem;

	Button buyBtn;
	buyBtn.type = ButtonType::BuyButton;
	buyBtn.action = ButtonAction::BuyItem;

	buttonsSystem.buttons.push_back(playBtn);
	buttonsSystem.buttons.push_back(feedBtn);
	buttonsSystem.buttons.push_back(sleepBtn);
	buttonsSystem.buttons.push_back(inventoryBtn);
	buttonsSystem.buttons.push_back(shopBtn);
	buttonsSystem.buttons.push_back(inventoryBackToBtn);
	buttonsSystem.buttons.push_back(shopBackToBtn);
	buttonsSystem.buttons.push_back(petBackToBtn);
	buttonsSystem.buttons.push_back(useBtn);
	buttonsSystem.buttons.push_back(buyBtn);

	for (size_t i = 0; i < buttonsSystem.buttons.size(); ++i) {
		
		ButtonVisual v;
		v.spriteIndex = i;
		v.currentScale = 0.f;
		v.targetScale = 0.f;
		v.hoverT = 0.f;
		v.currentPosition = buttonSprites[i].getPosition();
		v.targetPosition = v.currentPosition;

		buttonsSystem.visuals.push_back(v);

		buttonSprites[i].setOrigin(buttonSprites[i].getLocalBounds().size / 2.f);
	}
}

void Renderer::update_buttons(float dt) {

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	for (size_t i = 0; i < buttonsSystem.visuals.size(); ++i) {
		
		auto& v = buttonsSystem.visuals[i];
		v.hovered = buttonSprites[i].getGlobalBounds().contains(mousePos);

		float targetScale = v.targetScale;
		float target = targetScale * (v.hovered ? 1.07f : 1.f);

		v.hoverT += (target - v.hoverT) * 10.f * dt;

		sf::Sprite& sprite = buttonSprites[i];
		sprite.setScale({v.hoverT, v.hoverT});

		v.currentPosition += (v.targetPosition - v.currentPosition) * 15.f * dt;

		sprite.setPosition(v.currentPosition + buttonSprites[i].getLocalBounds().size / 2.f);
	}
}

void Renderer::add_popup(const std::string& str, const sf::Color& color, float x, float y, float lifeTime) {

	FloatingText popup(font);

	popup.text.setString(str);
	popup.text.setFillColor(color);
	popup.text.setCharacterSize(24);
	popup.text.setOutlineThickness(1.f);
	popup.text.setOutlineColor(sf::Color::White);

	popup.text.setPosition({x, y});

	popup.maxLifetime = lifeTime;

	popup.lifetime = 0.f;
	popup.active = true;

	popups.push_back(popup);
}

void Renderer::consume_popups(std::vector<Request>& popups) {

	for (auto& pop : popups) {
		sf::Color col = get_popup_color(pop.event);
		sf::Vector2f pos = get_popup_position(pop.pos);
		float lifeTime = get_popup_lifetime(pop.event);

		if (pop.event == GameEvent::ITEM_USED) {
			pos += {0.f, gap};
			gap += 30.f;
		}

		add_popup(pop.message, col, pos.x, pos.y, lifeTime);

		switch (pop.event) {
			case GameEvent::WARNING:
				soundMgr.play(SoundEffect::Warning);
				break;
			case GameEvent::EVENT:
				soundMgr.play(SoundEffect::Birthday);
				break;
			default: 
				break;
		}
	}

	gap = 0.f;
	popups.clear();
}

void Renderer::update_popups(float dt) {

	auto pops = drain_pending_popups(gameState);
	consume_popups(pops);

	for (FloatingText& popup : popups) {
		if (!popup.active) return;

		popup.lifetime += dt;
		popup.text.move(popup.velocity * dt);

		float ratio = 1.f - (popup.lifetime / popup.maxLifetime);

		sf::Color textC = popup.text.getFillColor();
		sf::Color outlineC = popup.text.getOutlineColor();
		textC.a = static_cast<std::uint8_t>(255 * ratio);
		outlineC.a = static_cast<std::uint8_t>(255 * ratio);

		popup.text.setFillColor(textC);
		popup.text.setOutlineColor(outlineC);

		// Delete
		if (popup.lifetime >= popup.maxLifetime) {
			popup.active = false;
			popups.erase(
				std::remove_if(popups.begin(), popups.end(), [](const FloatingText& pop) {
					return pop.active == false;
				}),
				popups.end()
			);
		}
	}
}

sf::Color Renderer::get_popup_color(GameEvent event) const {

	if (event == GameEvent::STAT_CHANGE) {
		return {92, 199, 108};
	} else if (event == GameEvent::EVENT) {
		return {230, 156, 105};
	} else if (event == GameEvent::WARNING) {
	    return {199, 92, 92};
	} else if (event == GameEvent::ITEM_USED) {
		return {92, 199, 108};
	}
}

sf::Vector2f Renderer::get_popup_position(PositionAnchor position) const {
	
	if (position == BottomLeft) {
		return {25.f, 778.f};
	} else if (position == TopCenter) {
		return {24.f, 358.f};
	} else if (position == LeftUp) {
		return {24.f, 446.f};
	} else if (position == LeftCenter) {
		return {24.f, 510.f};
	} else if (position == LeftBottom) {
		return {24.f, 574.f};
	} else if (position == BottomCenter) {
		return {168.f, 709.f};
	}
}

float Renderer::get_popup_lifetime(GameEvent event) const {
	
	if (event == GameEvent::STAT_CHANGE) {
		return 1.f;
	} else if (event == GameEvent::EVENT) {
		return 3.f;
	} else if (event == GameEvent::WARNING) {
	    return 2.f;
	} else if (event == GameEvent::ITEM_USED) {
		return 2.f;
	}
}

void Renderer::animate_subtitle(float dt) {

	float topY = 90.f;
	float bottomY = 110.f;

	sf::Color startColor = {0, 152, 220};
	sf::Color endColor = {71, 185, 231};

	float speed = 0.5f;

	static float progress;
	static bool movingDown;

	// Update progress
	if (movingDown) {
		progress += speed * dt;
	} else {
		progress -= speed * dt;
	}

	// Loop
	if (progress >= 1.f) {
		progress = 1.f;
		movingDown = false;
	}
	if (progress <= 0.f) {
		progress = 0.f;
		movingDown = true;
	}

	// Interpolate position
	float y = topY + (bottomY - topY) * progress;
	subtitle.setPosition({subtitle.getPosition().x, y});

	// Interpolate color
	sf::Color color;
	color.r = startColor.r + (endColor.r - startColor.r) * progress;
	color.g = startColor.g + (endColor.g - startColor.g) * progress;
	color.b = startColor.b + (endColor.b - startColor.b) * progress;
	
	subtitle.setFillColor(color);
}

void Renderer::draw_pet_screen(void) {
	
	window.draw(petBgSprite);
	window.draw(outerShadowSprite);
	window.draw(innerShadowSprite);

	// UI
	window.draw(title1);
	window.draw(title2);
	window.draw(subtitle);
	window.draw(nameRecSprite);
	window.draw(nameTitle);
	window.draw(nameStat);
	window.draw(healthRecSprite);
	window.draw(healthTitle);
	window.draw(healthStat);
	window.draw(hungerRecSprite);
	window.draw(hungerTitle);
	window.draw(hungerStat);
	window.draw(moodRecSprite);
	window.draw(moodTitle);
	window.draw(moodStat);
	
	window.draw(petIdleSprite);

	window.draw(energyRecSprite);
	window.draw(energyTitle);
	window.draw(energyStat);
	window.draw(ageRecSprite);
	window.draw(ageTitle);
	window.draw(ageStat);

	for (const FloatingText& popup : popups) {
		if (popup.active == true) window.draw(popup.text);
	}
}

void Renderer::draw_buttons(void) {
	
	for (size_t i = 0; i < buttonsSystem.buttons.size(); ++i) {
		window.draw(buttonSprites[i]);
	}
}

void Renderer::render(void) {
	
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();

	window.clear();

	update_buttons(dt);

	if (gameState.scene == Scene::Inventory) {
		draw_inventory();
		update_items(dt);
	} else if (gameState.scene == Scene::InventoryTooltip) {
		draw_inventory();
		update_items(dt);
		update_tooltip(dt);
	} else if (gameState.scene == Scene::Shop) {
		draw_shop();
		update_items(dt);
	} else if (gameState.scene == Scene::ShopTooltip) {
		draw_shop();
		update_items(dt);
		update_tooltip(dt);
	} else {
		update_pet_idle_animation();
		update_stats();
		update_popups(dt);
		animate_subtitle(dt);
		draw_pet_screen();
	}

	draw_buttons();

	window.display();
}

void Renderer::handle_events(void) {
	
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) window.close();

		sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

		// Button logic
		if (const sf::Event::MouseButtonPressed* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
			if (mouseEvent->button == sf::Mouse::Button::Left) {

				for (size_t i = 0; i < buttonsSystem.buttons.size() && i < buttonSprites.size(); ++i) {

					if (buttonSprites[i].getGlobalBounds().contains(mousePos)) {
						auto& a = buttonsSystem.buttons[i].action;
						
						switch (a) {
							case ButtonAction::PetPlay: {
								soundMgr.play(SoundEffect::Click);
								gameState.playPressed = true;
								pet_handle_input(gameState);		
							} break;
							case ButtonAction::PetFeed: {
								soundMgr.play(SoundEffect::Click);
								gameState.feedPressed = true;
								pet_handle_input(gameState);		
							} break;
							case ButtonAction::PetSleep: {
								soundMgr.play(SoundEffect::Click);
								gameState.sleepPressed = true;
								pet_handle_input(gameState);		
							} break;
							case ButtonAction::OpenInventory: {
								soundMgr.play(SoundEffect::Click);
								change_scene(Scene::Inventory);
							} break;
							case ButtonAction::OpenShop: {
								soundMgr.play(SoundEffect::Click);
								change_scene(Scene::Shop);
							} break;
							case ButtonAction::BackToInventory: {
								soundMgr.play(SoundEffect::Click);
								change_scene(Scene::Inventory);
							} break;
							case ButtonAction::BackToShop: {
								soundMgr.play(SoundEffect::Click);
								change_scene(Scene::Shop);
							} break;
							case ButtonAction::BackToPet: {
								soundMgr.play(SoundEffect::Click);
								change_scene(Scene::Pet);
							} break;
							case ButtonAction::UseItem: {
								use_item(gameState, selectedItemState.itemIndex);
								soundMgr.play(SoundEffect::ItemUsed);
								change_scene(Scene::Pet);
							} break;
							case ButtonAction::BuyItem: {
								soundMgr.play(SoundEffect::Click);
								change_scene(Scene::Shop);
							} break;
						}
					}
				}

				if (gameState.scene == Scene::Inventory || gameState.scene == Scene::Shop) {

					auto& items = (gameState.scene == Scene::Inventory) ? gameState.inventory : gameState.shop;

					for (size_t i = 0; i < items.size(); ++i) {

						if (slotSprites[i].getGlobalBounds().contains(mousePos)) {
							const Item& item = items[i];

							if (item.name.empty())
								continue;

							selectedItemState.itemIndex = static_cast<int>(i);
							selectedItemState.t = 0.f;

							change_scene(gameState.scene == Scene::Inventory ? Scene::InventoryTooltip : Scene::ShopTooltip);
							return;
						}
					}
				}
			} 
		}
	}
}

void Renderer::change_scene(const Scene& scene) {
	
	gameState.scene = scene;
	apply_scene_layout(scene);

	if (scene == Scene::Inventory || scene == Scene::InventoryTooltip || scene == Scene::Shop || scene == Scene::ShopTooltip)
		build_item_visuals();
}

void Renderer::draw_inventory(void) {
	
	window.draw(inventoryBgSprite);
	window.draw(inventoryTitle);
}

void Renderer::draw_shop(void) {

	window.draw(shopBgSprite);
	window.draw(shopTitle);
}

void Renderer::build_item_visuals(void) {

	auto& items = (gameState.scene == Scene::Inventory || gameState.scene == Scene::InventoryTooltip) ? gameState.inventory : gameState.shop;
	
	itemSystem.visuals.clear();
	itemSystem.visuals.resize(items.size() );
	std::cout << "SIZE: " << itemSystem.visuals.size() << '\n';

	if (gameState.scene == Scene::Inventory ||
		gameState.scene == Scene::InventoryTooltip)
	{
		std::cout << "INVENTORY\n";
	}
	else
	{
		std::cout << "SHOP\n";
	}

	for (size_t i = 0; i < itemSystem.visuals.size(); ++i) {

		std::cout << "for loop:\n";
		std::cout << "---> " << i;

		itemSystem.visuals[i].position = itemsPosition[i];

		std::cout << ": X=" << itemsPosition[i].x << "\n";

		itemSystem.visuals[i].hoverT = 0.f;
		itemSystem.visuals[i].hovered = false;
		itemSystem.visuals[i].selected = false;
	}
}

void Renderer::setup_items_position(void) {
	
	sf::Vector2f pos1({24.f, 88.f});
	sf::Vector2f pos2({168.f, 88.f});
	sf::Vector2f pos3({312.f, 88.f});
	sf::Vector2f pos4({456.f, 88.f});
	sf::Vector2f pos5({24.f, 223.f});
	sf::Vector2f pos6({168.f, 223.f});
	sf::Vector2f pos7({312.f, 223.f});
	sf::Vector2f pos8({456.f, 223.f});
	sf::Vector2f pos9({24.f, 358.f});
	sf::Vector2f pos10({168.f, 358.f});
	sf::Vector2f pos11({312.f, 358.f});
	sf::Vector2f pos12({456.f, 358.f});
	sf::Vector2f pos13({24.f, 493.f});
	sf::Vector2f pos14({168.f, 493.f});
	sf::Vector2f pos15({312.f, 493.f});
	sf::Vector2f pos16({456.f, 493.f});
	sf::Vector2f pos17({24.f, 628.f});
	sf::Vector2f pos18({168.f, 628.f});
	sf::Vector2f pos19({312.f, 628.f});
	sf::Vector2f pos20({468.f, 628.f});
	sf::Vector2f pos21({24.f, 763.f});
	sf::Vector2f pos22({168.f, 763.f});
	sf::Vector2f pos23({312.f, 763.f});
	sf::Vector2f pos24({456.f, 763.f});

	itemsPosition.push_back(pos1);
	itemsPosition.push_back(pos2);
	itemsPosition.push_back(pos3);
	itemsPosition.push_back(pos4);
	itemsPosition.push_back(pos5);
	itemsPosition.push_back(pos6);
	itemsPosition.push_back(pos7);
	itemsPosition.push_back(pos8);
	itemsPosition.push_back(pos9);
	itemsPosition.push_back(pos10);
	itemsPosition.push_back(pos11);
	itemsPosition.push_back(pos12);
	itemsPosition.push_back(pos13);
	itemsPosition.push_back(pos14);
	itemsPosition.push_back(pos15);
	itemsPosition.push_back(pos16);
	itemsPosition.push_back(pos17);
	itemsPosition.push_back(pos18);
	itemsPosition.push_back(pos19);
	itemsPosition.push_back(pos20);
	itemsPosition.push_back(pos21);
	itemsPosition.push_back(pos22);
	itemsPosition.push_back(pos23);
	itemsPosition.push_back(pos24);
}

void Renderer::setup_slot_sprites(void) {
	
	slotSprites.clear();

	sf::Vector2f offset(60.f, 60.f);

	for (int i = 0; i < MAX_ITEM_SLOTS; ++i) {
		sf::Sprite slot(itemSlotRecTexture);
		slot.setTexture(itemSlotRecTexture, true);

		sf::FloatRect bounds = slot.getLocalBounds();
		
		slot.setOrigin(bounds.size / 2.f);

		sf::Vector2f center = itemsPosition[i] + offset;
		slot.setPosition(center);

		slot.setScale({0.45f, 0.45f});

		slotSprites.push_back(slot);
	}
} 

void Renderer::setup_tooltip_window(void) {

	tooltipRecSprite.setTexture(tooltipRecTexture, true);
	tooltipRecSprite.setPosition({78.f, -130.f});

	itemName.setCharacterSize(16.f);
	itemName.setFillColor(sf::Color::White);
	itemName.setOutlineThickness(1.f);
	itemName.setOutlineColor({64, 64, 64});
}

void Renderer::update_items(float dt) {
	
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	Scene& scene = gameState.scene;

	float hide = (scene == Scene::InventoryTooltip || scene == Scene::ShopTooltip) ? 1.f : 0.f;

	for (size_t i = 0; i < itemSystem.visuals.size(); ++i) {

		auto& v = itemSystem.visuals[i];
		v.hovered = (scene == Scene::Inventory || scene == Scene::Shop) && slotSprites[i].getGlobalBounds().contains(mousePos);

		float target = hide ? 0.f : (v.hovered ? 0.55f : 0.45f);

		v.hoverT += (target - v.hoverT) * 10.f * dt;

		// slot
		sf::Sprite& slot = slotSprites[i];
		slot.setPosition(v.position + sf::Vector2f(60.f, 60.f));
		slot.setScale({v.hoverT, v.hoverT});

		// item
		const Item& item = (scene == Scene::Inventory) ? gameState.inventory[i] : gameState.shop[i];

		const Item* def = nullptr;

		for (auto& d : gameState.items)
			if (d.name == item.name) def = &d;

		if (!def) continue;

		sf::Sprite& sprite = itemSprites[def->index];

		sprite.setOrigin(sprite.getLocalBounds().size / 2.f);
		sprite.setPosition(slot.getPosition());
		sprite.setScale({v.hoverT, v.hoverT});

		window.draw(slot);
		window.draw(sprite);
	}
}

void Renderer::animate_item(sf::Sprite& rec, sf::Sprite& item, sf::Vector2f mousePos, float& currentScale, float dt) {

	static const float REC_TARGET_SCALE = 0.55f;
	static const float NORMAL_SCALE = 0.45f;
	static const float SPEED = 1.f;

	float target = rec.getGlobalBounds().contains(mousePos) ? REC_TARGET_SCALE : NORMAL_SCALE;

	if (currentScale < target) {
		currentScale += SPEED * dt;
		if (currentScale > target) currentScale = target;
	} else if (currentScale > target) {
		currentScale -= SPEED * dt;
		if (currentScale < target) currentScale = target;
	}

	rec.setScale({currentScale, currentScale});
	item.setScale({currentScale, currentScale});
}

void Renderer::update_tooltip(float dt) {
	
	if (gameState.scene != Scene::InventoryTooltip && gameState.scene != Scene::ShopTooltip) return;

	selectedItemState.t += dt * 2.5f;
	float t = std::min(selectedItemState.t, 1.f);

	auto& items = (gameState.scene == Scene::InventoryTooltip) ? gameState.inventory : gameState.shop;

	int i = selectedItemState.itemIndex;
	if (i < 0) return;
	if (i >= items.size()) return;

	const Item& item = items[i];

	const Item* def = nullptr;
	for (auto& d : gameState.items)
		if (d.name == item.name) def = &d;

	if (!def) return;

	// Tooltip from top
	sf::Vector2f tooltipTarget(78.f, 256.f);
	sf::Vector2f tooltipStart(78.f, -130.f);

	sf::Vector2f tooltipPos = tooltipStart + (tooltipTarget - tooltipStart) * t;

	tooltipRecSprite.setPosition(tooltipPos);

	itemName.setString(def->name);
	
	sf::FloatRect nameBounds = itemName.getLocalBounds();

	itemName.setOrigin({nameBounds.position.x + nameBounds.size.x / 2.f, nameBounds.position.y + nameBounds.size.y / 2.f});

	sf::FloatRect tooltipBounds = tooltipRecSprite.getGlobalBounds();

	itemName.setPosition({tooltipBounds.position.x + tooltipBounds.size.x / 2.f, tooltipBounds.position.y + tooltipBounds.size.y / 2.f - 38.f});

	window.draw(tooltipRecSprite);
	window.draw(itemName);

	// Item effects
	const std::vector<ItemEffect>& effects = def->itemEffects;

	float fontSize = 12.f;
	float gap = 4.f;

	float totalHeight = (fontSize * effects.size()) + ((effects.size() - 1) * gap);

	float centerY = tooltipBounds.position.y + tooltipBounds.size.y / 2.f;

	float startY = centerY - totalHeight / 2.f + fontSize / 2.f + gap;

	float posX = 234.f;

	for (size_t j = 0; j < effects.size(); ++j) {
		
		const ItemEffect& e = effects[j];

		sf::Text text(font);
		text.setCharacterSize(fontSize);
		text.setFillColor(sf::Color::White);
		text.setOutlineThickness(1.f);

		switch (e.type) {
			case ItemEffect::StatType::HEALTH: {
				text.setString("+" + std::to_string(e.value) + " health");
				text.setOutlineColor({199, 92, 92});
			} break;

			case ItemEffect::StatType::HUNGER: {
				text.setString("-" + std::to_string(e.value) + " hunger");
				text.setOutlineColor({199, 92, 92});
			} break;

			case ItemEffect::StatType::MOOD: {
				text.setString("-" + std::to_string(e.value) + " boredom");
				text.setOutlineColor({115, 192, 255});
			} break;

			case ItemEffect::StatType::ENERGY: {
				text.setString("+" + std::to_string(e.value) + " energy");
				text.setOutlineColor({92, 199, 108});
			} break;
		}

		float posY = startY + j * (fontSize + gap);

		text.setPosition({posX, posY});

		window.draw(text);
	}

	// Item from bottom
	sf::Vector2f bottomStart(300.f, 1100.f);
	sf::Vector2f centerTarget(300.f, 512.f);

	sf::Vector2f itemPos = bottomStart + (centerTarget - bottomStart) * t;

	const Item* defItem = def;

	sf::Sprite& sprite = itemSprites[defItem->index];

	sprite.setOrigin(sprite.getLocalBounds().size / 2.f);
	sprite.setPosition(itemPos);
	sprite.setScale({1.f, 1.f});

	window.draw(sprite);
}

/////////////////////// ADD NOTIFICATION AFTER ITEM USAGE

/*
void Renderer::update_shop(void) {
	
	for (int i = 0; i < gameState.inventoryItems.size(); ++i) {
		itemSlotRecSprite.setPosition({itemsPosition[i]});
		for (int j = 0; j < gameState.items.size(); ++j) {
			if (gameState.inventoryItems[i].name == gameState.items[j].name) {
				itemSprites[j].setPosition({itemsPosition[i].x + 20.f, itemsPosition[i].y + 20.f});
				window.draw(itemSlotRecSprite);
				window.draw(itemSprites[j]);

				sf::Text price(font);
				price.setString(std::to_string(gameState.items[j].price));
				price.setCharacterSize();
				window.draw();
			}
		}
	}
}*/