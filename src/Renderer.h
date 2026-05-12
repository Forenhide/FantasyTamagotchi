#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "PopupNotification.h"
#include "SoundManager.h"
#include <string>
#include <vector>

struct GameState;


struct Button {
	
	sf::Sprite sprite;
	sf::FloatRect hitbox;
	float currentScale = 1.f;
	bool isHovered = false;

	Button(const sf::Sprite& btnSprite) : sprite(btnSprite) {}
}; 

struct FloatingText {

	sf::Text text;
	bool active = false;
	float lifetime = 0.f;
	float maxLifetime = 1.f;
	sf::Vector2f velocity = {0.f, -30.f};

	FloatingText(const sf::Font& font) : text(font) {}
};


class Renderer {

public:
	Renderer(GameState& gameState);
	bool init(void);
	void render(void);
	void handle_events(void);
	bool is_open(void) const;
	void setup_button(Button& btn, sf::Texture& texture, float x, float y);
	void setup_title(void);
	void setup_stats_recs(sf::RectangleShape& rec, sf::Text& titleText, sf::Text& statText, const std::string& title, const std::string& stat, const sf::Color& color, const sf::Color& outline, float x, float y, float width, float height, int fontSizeTitle, int fontSizeStat, const sf::Color& fontColorTitle, const sf::Color& fontColorStat, const sf::Color& titleOutlineColor, const sf::Color& statOutlineColor);
	void setup_stats(void);
	void setup_animation(void);

private:
	GameState& gameState;
	SoundManager soundMgr;
	sf::RenderWindow window;
	sf::Vector2f mousePos;
	
	// Resources
	sf::Texture bgTexture;
	sf::Texture idleTexture;
	sf::Texture outerShadowTexture;
	sf::Texture innerShadowTexture;
	sf::Texture playBtnTexture;
	sf::Texture feedBtnTexture;
	sf::Texture sleepBtnTexture;
	sf::Texture inventoryBtnTexture;
	sf::Texture shopBtnTexture;
	sf::Sprite bgSprite;
	sf::Sprite idleSprite;
	sf::Sprite outerShadowSprite;
	sf::Sprite innerShadowSprite;
	sf::Sprite playBtnSprite;
	sf::Sprite feedBtnSprite;
	sf::Sprite sleepBtnSprite;
	sf::Sprite inventoryBtnSprite;
	sf::Sprite shopBtnSprite;
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

	std::vector<Button*> buttons;
	bool btnHovered{false};

	sf::Music music;

	std::vector<FloatingText> popups;

	void load_resources(void);
	void setup_sprites(void);
	
	void draw_pet_screen(void);
	void draw_inventory(void);
	void draw_shop(void);
	
	void animate_subtitle(float dt);
	void update_animation(void);
	void update_stats(void);

	void setup_buttons(void);
	void update_buttons(float dt);
	void update_button_hover(Button& btn, sf::Vector2f mousePos);
	void animate_button(Button& btn, float dt);

	void add_popup(const std::string& str, const sf::Color& color, float x, float y, float lifeTime);
	void update_popups(float dt);
	sf::Color get_popup_color(GameEvent event) const;
	sf::Vector2f get_popup_position(PositionAnchor position) const;
	float get_popup_lifetime(GameEvent event) const;
	void consume_popups(std::vector<Request>& popups);
};
