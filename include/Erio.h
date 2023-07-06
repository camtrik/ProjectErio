#pragma once
#include <SFML/Graphics.hpp>
#include <stack>

#include "Animation.h"
#include "Global.h"
#include "MapManager.h"
#include "Bomb.h"

class Erio
{
public:
	Erio();
	void draw(unsigned viewX, sf::RenderWindow &window);
	void update(unsigned viewX, MapManager& mapManager);
	float getX() const { return x; }
	float getVerticalSpeed () const { return verticalSpeed; }
	void setVerticalSpeed (const float p_verticalSpeed) { verticalSpeed = p_verticalSpeed; }
	void die(const unsigned short deathType);
	bool getDead() const { return dead; }
	bool isLoseLife() const { return loseLife; }
	void addBombToMap(const unsigned short p_x, const unsigned short p_y);
	void drawBombs(unsigned viewX, sf::RenderWindow& window);
	bool isHaveBomb() { return !bombsHave.empty(); }
	void getBomb();
	short getBombCount() const { return bombCount; }
	void throwBomb();
	void displayMessage(unsigned viewX, sf::RenderWindow& window);
	void getScore(unsigned int getScore) { killEnemyScore += getScore; }
	void reset(bool gameOverReset);
	sf::FloatRect getHitbox() const;
	

private:
	bool dead;
	bool loseLife;
	float x;
	float y;
	float verticalSpeed;
	float horizontalSpeed;
	short jumpTimer;
	short deathTimer;
	bool flip;
	std::vector<Cell> collisionObjects;

	std::vector<Bomb> bombs;
	std::vector<Bomb> bombThrow;

	sf::Sprite sprite;
	sf::Texture texture;
	sf::Texture idleTexture;
	sf::Texture jumpTexture;
	sf::Texture deathTexture;
	
	Animation walkAnimation;

	sf::SoundBuffer jumpSoundBuffer;
	sf::Sound jumpSound;
	sf::Music deadSound;

	std::stack<Bomb> bombsHave;
	short bombCount;
	sf::Font font;
	sf::Text bombMessageText;
	sf::Texture bombMessageTexture;
	sf::Sprite bombMessageSprite;

	unsigned short coinCount;
	sf::Text coinMessageText;
	sf::Texture coinMessageTexture;
	sf::Sprite coinMessageSprite;

	int killEnemyScore;
	int score;
	sf::Text scoreText;
};