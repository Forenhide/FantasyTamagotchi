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
	void setup_titles(void);
	void setup_stats_recs(sf::Sprite& rec, sf::Text& titleText, sf::Text& statText, const std::string& title, const std::string& stat, float x, float y, int fontSizeTitle, int fontSizeStat, const sf::Color& fontColorTitle, const sf::Color& fontColorStat, const sf::Color& titleOutlineColor, const sf::Color& statOutlineColor);
	void setup_stats(void);
	void setup_pet_idle_animation(void);
	void setup_pet_play_animation(void);

private:
	GameState& gameState;
	SoundManager soundMgr;
	sf::RenderWindow window;
	sf::Vector2f mousePos;
	
	// Resources
	// Textures
	sf::Texture petBgTexture;
	sf::Texture inventoryBgTexture;
	sf::Texture shopBgTexture;

	sf::Texture petIdleTexture;
	sf::Texture petPlayTexture;
	sf::Texture petHurtTexture;
	sf::Texture outerShadowTexture;
	sf::Texture innerShadowTexture;

	sf::Texture playBtnTexture;
	sf::Texture feedBtnTexture;
	sf::Texture sleepBtnTexture;
	sf::Texture inventoryBtnTexture;
	sf::Texture shopBtnTexture;
	sf::Texture backBtnTexture;
	sf::Texture useBtnTexture;
	sf::Texture buyBtnTexture;

	sf::Texture statRecTexture;
	sf::Texture smallStatRecTexture;
	sf::Texture itemSlotRecTexture;
	sf::Texture tooltipRecTexture;

	sf::Texture healingPotionTexture;
	sf::Texture joyPotionTexture;
	sf::Texture energyPotionTexture;
	sf::Texture universalPotionTexture;
	sf::Texture mochiTexture;
	sf::Texture pineappleTexture;
	sf::Texture teddyBearTexture;
	sf::Texture tetrisTexture;
	sf::Texture tiramisuTexture;
	sf::Texture yoYoTexture;

	// Sprites
	sf::Sprite petBgSprite;
	sf::Sprite inventoryBgSprite;
	sf::Sprite shopBgSprite;

	sf::Sprite petIdleSprite;
	sf::Sprite petPlaySprite;
	sf::Sprite petHurtSprite;
	sf::Sprite outerShadowSprite;
	sf::Sprite innerShadowSprite;

	sf::Sprite playBtnSprite;
	sf::Sprite feedBtnSprite;
	sf::Sprite sleepBtnSprite;
	sf::Sprite inventoryBtnSprite;
	sf::Sprite shopBtnSprite;
	sf::Sprite backBtnSprite;
	sf::Sprite useBtnSprite;
	sf::Sprite buyBtnSprite;

	sf::Sprite healthRecSprite;
	sf::Sprite hungerRecSprite;
	sf::Sprite moodRecSprite;
	sf::Sprite nameRecSprite;
	sf::Sprite ageRecSprite;
	sf::Sprite energyRecSprite;
	sf::Sprite itemSlotRecSprite;
	sf::Sprite tooltipRecSprite;

	sf::Sprite healingPotionSprite;
	sf::Sprite joyPotionSprite;
	sf::Sprite energyPotionSprite;
	sf::Sprite universalPotionSprite;
	sf::Sprite mochiSprite;
	sf::Sprite pineappleSprite;
	sf::Sprite teddyBearSprite;
	sf::Sprite tetrisSprite;
	sf::Sprite tiramisuSprite;
	sf::Sprite yoYoSprite;

	std::vector<sf::IntRect> idleFrames;
	std::vector<sf::IntRect> playFrames;
	float frameDuration = 0.13f;
	int currentFrame = 0;

	sf::Font font;
	sf::Text title1;
	sf::Text title2;
	sf::Text inventoryTitle;
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
	Button backBtn;
	Button useBtn;
	Button buyBtn;

	std::vector<Button*> buttons;
	bool btnHovered{false};

	sf::Music music;

	std::vector<FloatingText> popups;

	// Inventory
	std::vector<sf::Sprite> itemSprites;
	std::vector<sf::Vector2f> itemsPosition;
	std::vector<sf::Sprite> slotSprites;
	int selectedItem;
	sf::Text itemName;

	void load_resources(void);
	void setup_sprites(void);
	
	void draw_pet_screen(void);
	void draw_inventory(void);
	void draw_shop(void);
	
	void animate_subtitle(float dt);
	void update_pet_idle_animation(void);
	void update_pet_play_animation(void);
	void update_stats(void);

	void setup_buttons(void);
	void update_buttons(float dt);
	void update_button_hover(Button& btn, sf::Vector2f mousePos);
	void animate_button(Button& btn, float dt);

	// Inventory
	void update_inventory(float dt);
	void setup_items_position(void);
	void animate_item(sf::Sprite& rec, sf::Sprite& item, sf::Vector2f mousePos, float& currentScale, float dt);
	void update_tooltip(float dt);
	void show_tooltip(float dt);
	void setup_slot_sprites(void);
	void setup_tooltip_window(void);

	// Shop
	void update_shop(void);

	void add_popup(const std::string& str, const sf::Color& color, float x, float y, float lifeTime);
	void update_popups(float dt);
	sf::Color get_popup_color(GameEvent event) const;
	sf::Vector2f get_popup_position(PositionAnchor position) const;
	float get_popup_lifetime(GameEvent event) const;
	void consume_popups(std::vector<Request>& popups);
};
