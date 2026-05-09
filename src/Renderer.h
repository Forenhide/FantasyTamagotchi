#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct GameState;

struct Button {
	
	sf::RectangleShape shape;
	sf::Text text;
	float currentScale = 1.f;
	bool isHovered = false;
	sf::FloatRect hitbox;

	Button(const sf::Font& font) : text(font) {}
};

struct FloatingText {

	sf::Text text;
	bool active = false;
	float lifetime = 0.f;
	float maxLifeTime = 1.f;
	sf::Vector2f velocity = {0.f, -50.f};

	FloatingText(const sf::Font& font) : text(font) {}
};


class Renderer {

public:
	Renderer(GameState& gameState);
	bool init(void);
	void render(void);
	void handle_events(void);
	bool is_open(void) const;
	void setup_button(Button& btn, const std::string& text, const sf::Color& color, const sf::Color& outline, float x, float y, float width, float height, int fontSize, const sf::Color& fontColor);
	void setup_buttons(void);
	void setup_title(void);
	void setup_stats_recs(sf::RectangleShape& rec, sf::Text& titleText, sf::Text& statText, const std::string& title, const std::string& stat, const sf::Color& color, const sf::Color& outline, float x, float y, float width, float height, int fontSizeTitle, int fontSizeStat, const sf::Color& fontColorTitle, const sf::Color& fontColorStat, const sf::Color& titleOutlineColor, const sf::Color& statOutlineColor);
	void setup_stats(void);
	void setup_animation(void);

private:
	GameState& gameState;
	sf::RenderWindow window;
	sf::Vector2f mousePos;
	
	// Resources
	sf::Texture bgTexture;
	sf::Texture idleTexture;
	sf::Texture outerShadowTexture;
	sf::Texture innerShadowTexture;
	sf::Sprite bgSprite;
	sf::Sprite idleSprite;
	sf::Sprite outerShadowSprite;
	sf::Sprite innerShadowSprite;
	std::vector<sf::IntRect> idleFrames;
	float frameDuration = 0.13f;
	int currentFrame = 0;
	sf::Font font;
	sf::Text title1;
	sf::Text title2;
	sf::Text subtitle;
	sf::RectangleShape nameRec;
	sf::Text nameTitle;
	sf::Text nameStat;
	sf::RectangleShape healthRec;
	sf::Text healthTitle;
	sf::Text healthStat;
	sf::RectangleShape hungerRec;
	sf::Text hungerTitle;
	sf::Text hungerStat;
	sf::RectangleShape moodRec;
	sf::Text moodTitle;
	sf::Text moodStat;
	sf::RectangleShape energyRec;
	sf::Text energyTitle;
	sf::Text energyStat;
	sf::RectangleShape ageRec;
	sf::Text ageTitle;
	sf::Text ageStat;
	Button playBtn;
	Button feedBtn;
	Button sleepBtn;
	Button inventoryBtn;
	Button shopBtn;
	sf::Music music;
	FloatingText statPopup;
	bool btnHovered{false};
	std::vector<Button*> buttons;

	void load_resources(void);
	void setup_sprites(void);
	void update_animation(void);
	void update_stats(void);
	void draw_pet_screen(void);
	void draw_inventory(void);
	void draw_shop(void);
	void show_stat_popup(const std::string& str, const sf::Color& color);
	void update_popup(float dt);
	void animate_subtitle(float dt);
	void update_buttons(float dt);
	void update_button_hover(Button& btn, sf::Vector2f mousePos);
	void animate_button(Button& btn, float dt);
};