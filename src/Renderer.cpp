#include "Renderer.h"
#include "GameState.h"
#include <windows.h>
#include <iostream>
#include <algorithm>


Renderer::Renderer(GameState& gameState) : 
	gameState(gameState),
	bgSprite(bgTexture),
	idleSprite(idleTexture),
	outerShadowSprite(outerShadowTexture), innerShadowSprite(innerShadowTexture), playBtn(font),
	feedBtn(font),
	sleepBtn(font),
	inventoryBtn(font),
	shopBtn(font),
	title1(font, "FANTASY", 48),
	title2(font, "TAMAGOTCHI", 48),
	subtitle(font, "* Kitty Edition *", 20),
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
	ageStat(font)
{}

bool Renderer::init(void) {
	
	window.create(sf::VideoMode({600, 1024}), "Fantasy Tamagotchi (Kitty Edition)", sf::Style::Close | sf::Style::Titlebar);

	load_resources();
	setup_sprites();
	setup_buttons();
	setup_title();
	setup_stats();
	setup_animation();

	music.play();
	music.setVolume(3.f);
	music.setLooping(true);

	return true;
}

bool Renderer::is_open(void) const {
	
	return window.isOpen();
}

void Renderer::load_resources(void) {
	
	if (!bgTexture.loadFromFile("assets/background.jpg")) throw std::runtime_error("bg");
	if (!idleTexture.loadFromFile("assets/idle.png")) throw std::runtime_error("pet");
	if (!outerShadowTexture.loadFromFile("assets/shadow1.png")) throw std::runtime_error("outer shadow");
	if (!innerShadowTexture.loadFromFile("assets/shadow2.png")) throw std::runtime_error("inner shadow");
	if (!font.openFromFile("assets/PressStart2P-Regular.ttf")) throw std::runtime_error("font");
	if (!music.openFromFile("assets/Music.mp3")) throw std::runtime_error("music");
}

void Renderer::setup_sprites(void) {
	
	// Set textures
	bgSprite.setTexture(bgTexture, true);
	idleSprite.setTexture(idleTexture, true);
	outerShadowSprite.setTexture(outerShadowTexture, true);
	innerShadowSprite.setTexture(innerShadowTexture, true);

	// Adjust scale
	idleSprite.setScale({15.f, 15.f});

	// Adjust positions
	idleSprite.setPosition({80.f, 420.f});
	outerShadowSprite.setPosition({133.f, 780.f});
	innerShadowSprite.setPosition({182.f, 795.f});
}

void Renderer::setup_stats(void) {

	setup_stats_recs(nameRec, nameTitle, nameStat, "Name", gameState.name, {255, 255, 255, 130}, {255, 255, 255}, 24.f, 164.f, 264.f, 74.f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {115, 192, 255});
	setup_stats_recs(healthRec, healthTitle, healthStat, "Health", std::to_string(gameState.health), {255, 255, 255, 130}, {255, 255, 255}, 24.f, 246.f, 264.f, 74.f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {92, 199, 108});
	setup_stats_recs(hungerRec, hungerTitle, hungerStat, "Hunger", gameState.hunger, {255, 255, 255, 130}, {255, 255, 255}, 312.f, 164.f, 264.f, 74.f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {92, 199, 108});
	setup_stats_recs(moodRec, moodTitle, moodStat, "Mood", gameState.mood, {255, 255, 255, 130}, {255, 255, 255}, 312.f, 246.f, 264.f, 74.f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {199, 92, 92});
	setup_stats_recs(energyRec, energyTitle, energyStat, "Energy", std::to_string(gameState.energy), {255, 255, 255, 130}, {255, 255, 255}, 24.f, 818.f, 132.f, 74.f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {92, 199, 108});
	setup_stats_recs(ageRec, ageTitle, ageStat, "Age", std::to_string(gameState.age), {255, 255, 255, 130}, {255, 255, 255}, 444.f, 818.f, 132.f, 74.f, 16, 14, {255, 255, 255}, {255, 255, 255}, {64, 64, 64}, {115, 192, 255});
}

void Renderer::setup_stats_recs(sf::RectangleShape& rec, sf::Text& titleText, sf::Text& statText, const std::string& title, const std::string& stat, const sf::Color& color, const sf::Color& outline, float x, float y, float width, float height, int fontSizeTitle, int fontSizeStat, const sf::Color& fontColorTitle, const sf::Color& fontColorStat, const sf::Color& titleOutlineColor, const sf::Color& statOutlineColor) {

	// Shape
	rec.setSize({width, height});
	rec.setPosition({x, y});
	rec.setFillColor(color);
	rec.setOutlineColor(outline);
	rec.setOutlineThickness(2.f);

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

void Renderer::setup_title(void) {
	
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
}

void Renderer::setup_buttons(void) {
	
	setup_button(playBtn, "Play", {255, 255, 255}, {115, 192, 255}, 24.f, 908.f, 178.f, 46.f, 20, {115, 192, 255});
	setup_button(feedBtn, "Feed", {255, 255, 255}, {115, 192, 255}, 210.f, 908.f, 178.f, 46.f, 20, {115, 192, 255});
	setup_button(sleepBtn, "Sleep", {255, 255, 255}, {115, 192, 255}, 397.f, 908.f, 178.f, 46.f, 20, {115, 192, 255});
	setup_button(inventoryBtn, "Inventory", {255, 255, 255}, {255, 174, 174}, 24.f, 962.f, 216.f, 46.f, 20, {255, 174, 174});
	setup_button(shopBtn, "Shop", {255, 255, 255}, {255, 174, 174}, 360.f, 962.f, 216.f, 46.f, 20, {255, 174, 174});
}

void Renderer::setup_button(Button& btn, const std::string& text, const sf::Color& color, const sf::Color& outline, float x, float y, float width, float height, int fontSize, const sf::Color& fontColor) {

	// Shape
	btn.shape.setSize({width, height});
	btn.shape.setFillColor(color);
	btn.shape.setOutlineColor(outline);
	btn.shape.setOutlineThickness(2.f);

	// Text
	btn.text.setString(text);
	btn.text.setCharacterSize(fontSize);
	btn.text.setFillColor(fontColor);

	// Center text
	sf::FloatRect text_bounds = btn.text.getLocalBounds();

	btn.shape.setOrigin(btn.shape.getSize() / 2.f);
	btn.shape.setScale({btn.currentScale, btn.currentScale});
	btn.shape.setPosition({x + width / 2.f, y + height / 2.f});

	sf::Vector2f pos = btn.shape.getPosition();

	btn.text.setOrigin({text_bounds.position.x + text_bounds.size.x / 2.f, text_bounds.position.y + text_bounds.size.y / 2.f});
	btn.text.setPosition({pos.x, pos.y});

	btn.hitbox.size = {width, height};
	btn.hitbox.position = {x, y};

	buttons.push_back(&btn);
}

void Renderer::setup_animation(void) {
	
	idleFrames.push_back(sf::IntRect({0, 0}, {29, 28}));
	idleFrames.push_back(sf::IntRect({29, 0}, {28, 28}));
	idleFrames.push_back(sf::IntRect({57, 0}, {28, 28}));
	idleFrames.push_back(sf::IntRect({85, 0}, {28, 28}));
	idleFrames.push_back(sf::IntRect({113, 0}, {28, 28}));
	idleFrames.push_back(sf::IntRect({141, 0}, {28, 28}));
	idleFrames.push_back(sf::IntRect({169, 0}, {28, 28}));
	idleFrames.push_back(sf::IntRect({197, 0}, {29, 28}));

	idleSprite.setTexture(idleTexture);
	idleSprite.setTextureRect(idleFrames[0]);
}

void Renderer::update_animation(void) {
	
	static sf::Clock clock;

	if (clock.getElapsedTime().asSeconds() >= frameDuration) {
		currentFrame = (currentFrame + 1) % idleFrames.size();
		idleSprite.setTextureRect(idleFrames[currentFrame]);
		clock.restart();
	}
}

void Renderer::update_stats(void) {
	
	healthStat.setString(std::to_string(gameState.health));
	hungerStat.setString(gameState.hunger);
	moodStat.setString(gameState.mood);
	energyStat.setString(std::to_string(gameState.energy));
	ageStat.setString(std::to_string(gameState.age));
}

void Renderer::add_popup(const std::string& str, const sf::Color& color, float x, float y) {

	FloatingText popup(font);

	popup.text.setString(str);
	popup.text.setFillColor(color);
	popup.text.setCharacterSize(24);
	popup.text.setOutlineThickness(1.f);
	popup.text.setOutlineColor(sf::Color::White);

	popup.text.setPosition({x, y});

	popup.lifetime = 0.f;
	popup.active = true;

	popups.push_back(popup);
}

void Renderer::consume_popups(std::vector<Request>& popups) {

	for (const auto& pop : popups) {
		sf::Color col = get_popup_color(pop.event);
		sf::Vector2f pos = get_popup_position(pop.pos);

		add_popup(pop.message, col, pos.x, pos.y);
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

		float ratio = 1.f - (popup.lifetime / popup.maxLifeTime);

		sf::Color textC = popup.text.getFillColor();
		sf::Color outlineC = popup.text.getOutlineColor();
		textC.a = static_cast<std::uint8_t>(255 * ratio);
		outlineC.a = static_cast<std::uint8_t>(255 * ratio);

		popup.text.setFillColor(textC);
		popup.text.setOutlineColor(outlineC);

		if (popup.lifetime >= popup.maxLifeTime) {
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
	} else if (position == LeftCenter) {
		return {24.f, 548.f};
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

void Renderer::update_buttons(float dt) {

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	update_button_hover(playBtn, mousePos);
	update_button_hover(feedBtn, mousePos);
	update_button_hover(sleepBtn, mousePos);
	update_button_hover(inventoryBtn, mousePos);
	update_button_hover(shopBtn, mousePos);

	animate_button(playBtn, dt);
	animate_button(feedBtn, dt);
	animate_button(sleepBtn, dt);
	animate_button(inventoryBtn, dt);
	animate_button(shopBtn, dt);
}

void Renderer::update_button_hover(Button& btn, sf::Vector2f mousePos) {

	bool hovering = btn.hitbox.contains(mousePos);
	if (hovering != btn.isHovered) {
		btn.isHovered = hovering;
	}
}

void Renderer::animate_button(Button& btn, float dt) {

	static const float TARGET_SCALE = 1.1f;
	static const float NORMAL_SCALE = 1.0f;
	static const float SPEED = 1.f;

	float target = btn.isHovered ? TARGET_SCALE : NORMAL_SCALE;

	if (btn.currentScale < target) {
		btn.currentScale += SPEED * dt;
		if (btn.currentScale > target) btn.currentScale = target;
	} else if (btn.currentScale > target) {
		btn.currentScale -= SPEED * dt;
		if (btn.currentScale < target) btn.currentScale = target;
	}

	btn.shape.setScale({btn.currentScale, btn.currentScale});

	sf::Vector2f pos = btn.shape.getPosition();
	btn.text.setPosition(pos);
}

void Renderer::draw_pet_screen(void) {
	
	window.draw(bgSprite);
	window.draw(outerShadowSprite);
	window.draw(innerShadowSprite);

	// UI
	window.draw(title1);
	window.draw(title2);
	window.draw(subtitle);
	for (Button* btn : buttons) {
		if (!(btn->isHovered)) {
			window.draw(btn->shape);
			window.draw(btn->text);
		}
	}
	for (Button* btn : buttons) {
		if (btn->isHovered) {
			window.draw(btn->shape);
			window.draw(btn->text);
		}
	}
	window.draw(nameRec);
	window.draw(nameTitle);
	window.draw(nameStat);
	window.draw(healthRec);
	window.draw(healthTitle);
	window.draw(healthStat);
	window.draw(hungerRec);
	window.draw(hungerTitle);
	window.draw(hungerStat);
	window.draw(moodRec);
	window.draw(moodTitle);
	window.draw(moodStat);
	window.draw(energyRec);
	window.draw(energyTitle);
	window.draw(energyStat);
	window.draw(ageRec);
	window.draw(ageTitle);
	window.draw(ageStat);

	window.draw(idleSprite);

	for (const FloatingText& popup : popups) {
		if (popup.active == true) window.draw(popup.text);
	}
}

void Renderer::render(void) {
	
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();

	update_animation();
	update_stats();
	update_buttons(dt);
	update_popups(dt);
	animate_subtitle(dt);


	window.clear();

	if (gameState.inventoryOpened) {
		draw_inventory();
	} else if (gameState.shopOpened) {
		draw_shop();
	} else {
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
				if (playBtn.shape.getGlobalBounds().contains(mousePos)) {
					gameState.playPressed = true;
					pet_handle_input(gameState);
					//add_popup("+" + std::to_string(gameState.newStat), sf::Color({92, 199, 108}), 25.f, 778.f);
				}

				if (feedBtn.shape.getGlobalBounds().contains(mousePos)) {
					gameState.feedPressed = true;
					pet_handle_input(gameState);
					//add_popup("+" + std::to_string(gameState.newStat), sf::Color({92, 199, 108}), 25.f, 778.f);
				}

				if (sleepBtn.shape.getGlobalBounds().contains(mousePos)) {
					gameState.sleepPressed = true;
					pet_handle_input(gameState);
					//add_popup("+" + std::to_string(gameState.newStat), sf::Color({92, 199, 108}), 25.f, 778.f);
				}


				if (inventoryBtn.shape.getGlobalBounds().contains(mousePos)) {
					gameState.inventoryOpened = true;
				}

				if (shopBtn.shape.getGlobalBounds().contains(mousePos)) {
					gameState.shopOpened = true;
				}
			}
		}

		// Warnings
		//if (gameState.isAlive) {
		//	if (gameState.birthday) add_popup("It's my birthday today!", sf::Color({230, 156, 105}), 24.f, 358.f);
		//	if (gameState.isIll) add_popup("I feel sick...", sf::Color({199, 92, 92}), 24.f, 548.f);
		//} else {}

		// Inventory / Shop
	}
}

void Renderer::draw_inventory(void) {
	

}

void Renderer::draw_shop(void) {


}




/*
void Renderer::render_pet(void) {

	pet_update_warnings(game_state);
	pet_update_age(game_state);

	process_reaction_art();
	process_warnings();

	display_item_message();
	
}


void Renderer::process_reaction_art(void) {

	if (game_state.isAlive) {
		if (game_state.birthday) {
			std::cout << " It's my birthday today! I am " << game_state.age << " years old!";
		}
		if (game_state.isIll) {
			pet_set_art(game_state, "🤧");;
			pet_set_reaction(game_state, "I feel sick...");
		}
	} else {
		pet_set_art(game_state, "💀");;
		pet_set_reaction(game_state, "");
		std::cout << '\t' << game_state.name << " has died...";
	}
	std::cout << "\n\n";

	std::cout << "\t\t   " << game_state.art;
	std::cout << '\t' << game_state.reaction << '\n';
}

void Renderer::process_warnings(void) {

	if (!(game_state.warnings.empty())) {
		for (int i = 0; i < game_state.warnings.size(); i++) {
			std::cout << game_state.warnings[i] << '\n';
		}
		std::cout << '\n';
	}
	else {
		std::cout << "\n\n";
	}
}





void Renderer::render_inventory(void) {

	system("cls");

	std::cout << "INVENTORY" << "\n\n";

	for (int i = 0; i < game_state.inventoryItems.size(); i++) {
		std::cout << "[" << i + 1 << "] " << game_state.inventoryItems[i].name << '\n';
	}
	
	std::cout << "\n[0] Exit";
	std::cout << "\n\n";
}

void Renderer::display_item_message(void) const {

	std::cout << game_state.inventoryMessage << "\n\n";
}

void Renderer::render_shop(void) {

	system("cls");
	
	std::cout << "SHOP" << "\n\n";
	std::cout << "Balance: $" << game_state.balance << "\n\n\n";

	for (int i = 0; i < game_state.shopItems.size(); i++) {
		std::cout << "[" << i + 1 << "] " << game_state.shopItems[i].name << " ($" << game_state.shopItems[i].price << ")\n";
	}

	std::cout << "\n[0] Exit";

	if (!(game_state.transactionMessage.empty()))
		std::cout << "\n\n" << game_state.transactionMessage;

	std::cout << "\n\n";
}
*/
