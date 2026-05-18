#include "Renderer.h"
#include "GameState.h"
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
	backBtnSprite(backBtnTexture),
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
	playBtn(playBtnSprite),
	feedBtn(feedBtnSprite),
	sleepBtn(sleepBtnSprite),
	inventoryBtn(inventoryBtnSprite),
	shopBtn(shopBtnSprite),
	backBtn(backBtnSprite),
	useBtn(useBtnSprite),
	buyBtn(buyBtnSprite),

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

	selectedItem(-1),
	itemName(font)
{}

bool Renderer::init(void) {
	
	window.create(sf::VideoMode({600, 1024}), "Fantasy Tamagotchi (Kitty Edition)", sf::Style::Close | sf::Style::Titlebar);

	load_resources();
	setup_sprites();
	setup_buttons();
	setup_titles();
	setup_stats();
	setup_pet_idle_animation();
	setup_pet_play_animation();

	// Inventory
	setup_items_position();
	setup_slot_sprites();
	setup_tooltip_window();

	if(!soundMgr.init()) throw std::runtime_error("sound manager init");

	music.play();
	music.setVolume(0.5f);
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
	backBtnSprite.setTexture(backBtnTexture, true);
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
	inventoryTitle.setOutlineColor({255, 255, 255});
	inventoryTitle.setOutlineThickness(1.f);
	inventoryTitle.setPosition({84.f, 16.f});

	// Shop
}

void Renderer::setup_buttons(void) {
	
	setup_button(playBtn, playBtnTexture, 24.f, 908.f);
	setup_button(feedBtn, feedBtnTexture, 210.f, 908.f);
	setup_button(sleepBtn, sleepBtnTexture, 397.f, 908.f);
	setup_button(inventoryBtn, inventoryBtnTexture, 24.f, 962.f);
	setup_button(shopBtn, shopBtnTexture, 360.f, 962.f);

	setup_button(backBtn, backBtnTexture, 397.f, 962.f);
	setup_button(useBtn, useBtnTexture, 344.f, 668.f);
	setup_button(buyBtn, buyBtnTexture, 344.f, 668.f);
}

void Renderer::setup_button(Button& btn, sf::Texture& texture, float x, float y) {

	btn.sprite.setTexture(texture, true);

	btn.hitbox.size = btn.sprite.getGlobalBounds().size;
	btn.hitbox.position = {x, y};

	btn.sprite.setOrigin(btn.sprite.getGlobalBounds().size / 2.f);
	btn.sprite.setScale({btn.currentScale, btn.currentScale});
	btn.sprite.setPosition({x + btn.sprite.getGlobalBounds().size.x / 2.f, y + btn.sprite.getGlobalBounds().size.y / 2.f});

	if (&btn != &backBtn && &btn != &useBtn && &btn != &buyBtn) buttons.push_back(&btn);
}

void Renderer::update_buttons(float dt) {

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	update_button_hover(playBtn, mousePos);
	update_button_hover(feedBtn, mousePos);
	update_button_hover(sleepBtn, mousePos);
	update_button_hover(inventoryBtn, mousePos);
	update_button_hover(shopBtn, mousePos);
	update_button_hover(backBtn, mousePos);
	update_button_hover(useBtn, mousePos);
	update_button_hover(buyBtn, mousePos);

	animate_button(playBtn, dt);
	animate_button(feedBtn, dt);
	animate_button(sleepBtn, dt);
	animate_button(inventoryBtn, dt);
	animate_button(shopBtn, dt);
	animate_button(backBtn, dt);
	animate_button(useBtn, dt);
	animate_button(buyBtn, dt);
}

void Renderer::update_button_hover(Button& btn, sf::Vector2f mousePos) {

	bool hovering = btn.hitbox.contains(mousePos);
	if (hovering != btn.isHovered) btn.isHovered = hovering;
}

void Renderer::animate_button(Button& btn, float dt) {

	static const float NORMAL_SCALE = 1.f;
	static const float TARGET_SCALE = 1.1f;
	static const float SPEED = 1.f;

	float target = btn.isHovered ? TARGET_SCALE : NORMAL_SCALE;

	if (btn.currentScale < target) {
		btn.currentScale += SPEED * dt;
		if (btn.currentScale > target) btn.currentScale = target;
	} else if (btn.currentScale > target) {
		btn.currentScale -= SPEED * dt;
		if (btn.currentScale < target) btn.currentScale = target;
	}

	btn.sprite.setScale({btn.currentScale, btn.currentScale});
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

	for (const auto& pop : popups) {
		sf::Color col = get_popup_color(pop.event);
		sf::Vector2f pos = get_popup_position(pop.pos);
		float lifeTime = get_popup_lifetime(pop.event);

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
	}
}

float Renderer::get_popup_lifetime(GameEvent event) const {
	
	if (event == GameEvent::STAT_CHANGE) {
		return 1.f;
	} else if (event == GameEvent::EVENT) {
		return 3.f;
	} else if (event == GameEvent::WARNING) {
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
	for (Button* btn : buttons) {
		if (!(btn->isHovered)) window.draw(btn->sprite);
	}
	for (Button* btn : buttons) {
		if (btn->isHovered) window.draw(btn->sprite);
	}
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

void Renderer::render(void) {
	
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();

	update_buttons(dt);

	window.clear();

	if (gameState.scene == Scene::Inventory) {
		draw_inventory();
		update_inventory(dt);
	} else if (gameState.scene == Scene::InventoryTooltip) {
			draw_inventory();
			update_inventory(dt);
			update_tooltip(dt);
	} else if (gameState.scene == Scene::Shop) {
		draw_shop();
	} else {
		update_pet_idle_animation();
		update_stats();
		update_popups(dt);
		animate_subtitle(dt);
		draw_pet_screen();
	}

	window.display();
}

void Renderer::handle_events(void) {
	
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) window.close();

		sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

		// Button logic
		if (const sf::Event::MouseButtonPressed* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
			if (mouseEvent->button == sf::Mouse::Button::Left) {
				if (gameState.scene == Scene::Pet) {
					if (playBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						gameState.playPressed = true;
						pet_handle_input(gameState);
					}

					if (feedBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						gameState.feedPressed = true;
						pet_handle_input(gameState);
					}

					if (sleepBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						gameState.sleepPressed = true;
						pet_handle_input(gameState);
					}

					if (inventoryBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						gameState.scene = Scene::Inventory;
					}

					if (shopBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						gameState.scene = Scene::Shop;
					}
				} else if (gameState.scene == Scene::Inventory) {
					if (backBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						gameState.scene = Scene::Pet;
					}

					for (int i = 0; i < gameState.inventoryItems.size(); ++i) {

						if (slotSprites[i].getGlobalBounds().contains(mousePos)) {
							selectedItem = i;
							gameState.scene = Scene::InventoryTooltip;
							break;
						}
					}
				} else if (gameState.scene == Scene::InventoryTooltip) {
					if (backBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						gameState.scene = Scene::Inventory;
					}

					if (useBtn.sprite.getGlobalBounds().contains(mousePos)) {
						soundMgr.play(SoundEffect::Click);
						use_item(gameState, selectedItem);
						gameState.scene = Scene::Pet;
					}
				}
			} 
		}
	}
}

void Renderer::draw_inventory(void) {
	
	window.draw(inventoryBgSprite);
	window.draw(inventoryTitle);
	window.draw(tooltipRecSprite);
	window.draw(itemName);
	
	if (!(backBtn.isHovered)) window.draw(backBtn.sprite);
	if (backBtn.isHovered) window.draw(backBtn.sprite);
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

void Renderer::update_inventory(float dt) {
	
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	for (int i = 0; i < gameState.inventoryItems.size(); ++i) {

		Item& item = gameState.inventoryItems[i];

		for (int j = 0; j < gameState.items.size(); ++j) {
			if (item.name == gameState.items[j].name) {
				sf::Sprite& sprite = itemSprites[j];
				sf::Sprite& slot = slotSprites[i];

				sf::FloatRect slotBounds = slot.getGlobalBounds();
				sprite.setOrigin(sprite.getLocalBounds().size / 2.f);
				sprite.setPosition({slotBounds.position.x + slotBounds.size.x / 2.f, slotBounds.position.y + slotBounds.size.y / 2.f});

				animate_item(slot, sprite, mousePos, item.currentScale, dt);

				if (item.currentScale <= 0.02) continue;

				window.draw(slot);
				window.draw(sprite);
			}
		}
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
	
	if (gameState.scene == Scene::InventoryTooltip) {
		show_tooltip(dt);
	} else if (gameState.scene == Scene::ShopTooltip) {
		// same thing
	}
}

void Renderer::show_tooltip(float dt) {
	
	static const float TARGET_SCALE = 0.01f;

	static const float ITEM_TARGET_SCALE = 1.0f;

	static const sf::Vector2f TARGET_POS = {168.f, 380.f};
	static const float TARGET_TOOLTIP_POS_Y = 236.f;

	static const float SCALE_SPEED = 3.f;
	static const float MOVE_SPEED = 500.f;

	for (int i = 0; i < gameState.inventoryItems.size(); ++i) {
		Item& item = gameState.inventoryItems[i];

		for (int j = 0; j < gameState.items.size(); ++j) {
			if (item.name != gameState.items[j].name) continue;

			sf::Sprite& sprite = itemSprites[j];
			sf::Sprite& slot = slotSprites[i];

			float scale = slotSprites[i].getScale().x;

			if (i != selectedItem) {
				item.currentScale -= SCALE_SPEED * dt;
				if (item.currentScale < TARGET_SCALE) item.currentScale = TARGET_SCALE;
				slot.setScale({item.currentScale, item.currentScale});
				sprite.setScale({item.currentScale, item.currentScale});
			} else {
				item.currentScale += SCALE_SPEED * dt;
				if (item.currentScale > ITEM_TARGET_SCALE) item.currentScale = ITEM_TARGET_SCALE;
				slot.setScale({item.currentScale, item.currentScale});
				sprite.setScale({item.currentScale, item.currentScale});

				sf::Vector2f direction = TARGET_POS - slot.getPosition() + slot.getLocalBounds().size / 2.f;
				float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

				if (length > 1.f) {
				
					direction /= length;

					slot.move(direction * MOVE_SPEED * dt);
				}
				
				// Tooltip rectangle
				float tooltipDirection = TARGET_TOOLTIP_POS_Y - tooltipRecSprite.getPosition().y;

				if (std::abs(tooltipDirection) <= MOVE_SPEED * dt) {
					tooltipRecSprite.setPosition({tooltipRecSprite.getPosition().x, TARGET_TOOLTIP_POS_Y});
					
				} else {
					tooltipRecSprite.move({0.f, (tooltipDirection > 0 ? MOVE_SPEED * dt : -MOVE_SPEED * dt)});
					itemName.move({0.f, (tooltipDirection > 0 ? MOVE_SPEED * dt : -MOVE_SPEED * dt)});
				}

				// Tooltip stat name
				itemName.setString(item.name);

				sf::FloatRect nameBounds = itemName.getLocalBounds();

				itemName.setOrigin({nameBounds.position.x + nameBounds.size.x / 2.f, nameBounds.position.y + nameBounds.size.y / 2.f});

				sf::FloatRect tooltipBounds = tooltipRecSprite.getGlobalBounds();

				itemName.setPosition({tooltipBounds.position.x + tooltipBounds.size.x / 2.f, tooltipBounds.position.y + tooltipBounds.size.y / 2.f - 38.f});

				// Tooltip stat value
				std::vector<ItemEffect>& effects = item.itemEffects;
				float fontSize = 12.f;
				float gap = 4.f;
				float totalHeight = (fontSize * effects.size()) + ((effects.size() - 1) * gap);
				float centerY = tooltipBounds.position.y + tooltipBounds.size.y / 2.f;
				float startY = centerY - totalHeight / 2.f + fontSize / 2.f + gap;
				float posX = 234.f;

				std::vector<sf::Text> effectTexts;

				for (int i = 0; i < effects.size(); ++i) {

					const ItemEffect& e = effects[i];

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

					float posY = startY + i * (fontSize + gap);
					text.setPosition({posX, posY});

					window.draw(text);
				}

				// Buttons
				// [Back]


				// [Use]
			}
		}
	}
}

void Renderer::draw_shop(void) {


}

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