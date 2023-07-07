#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "Entity.h"

class Bomb : public Entity
{
public:
	Bomb(const float p_x, const float p_y, bool p_isThrow=false, short p_isRight=0);
	void draw(const unsigned viewX, sf::RenderWindow& window);
	void update(const unsigned viewX, MapManager& mapManager, Erio& erio);
	void die(const unsigned char deathType);
	float getX() const { return x; }

	sf::FloatRect getHitbox() const;


private:
	// dispear after a while after death
	short deathTimer;
	// if the bomb is thrown by erio and the direction, -1 : left, 1 : right
	bool isThrow;
	short direction;
	float startY;
	
	std::vector<Cell> collisionObjects;

	sf::Sprite sprite;
	sf::Texture texture;
};