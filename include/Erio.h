#pragma once
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Global.h"
#include "MapManager.h"

class Erio
{
public:
	Erio();
	void draw(sf::RenderWindow &window);
	void update(MapManager& mapManager);
	float getX() const { return x; }
	float getVerticalSpeed () const { return verticalSpeed; }
	void setVerticalSpeed (const float p_verticalSpeed) { verticalSpeed = p_verticalSpeed; }
	void die(const unsigned short deathType);
	bool getDead() const { return dead; }


	sf::FloatRect getHitbox() const;
	

private:
	bool dead;
	float x;
	float y;
	float verticalSpeed;
	float horizontalSpeed;
	short jumpTimer;
	short deathTimer;
	bool flip;
	std::vector<Cell> collisionObjects;

	sf::Sprite sprite;
	sf::Texture texture;
	Animation walkAnimation;
};