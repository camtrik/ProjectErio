#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "Entity.h"

class Slime : public Entity
{
public:
	Slime(const float p_x, const float p_y);
	void draw(const unsigned viewX, sf::RenderWindow& window);
	void update(const unsigned viewX, MapManager& mapManager, Erio& erio, const std::vector<std::shared_ptr<Entity>>& enemies);
	void die(const unsigned char deathType);
	float getX() const { return x; }

	sf::FloatRect getHitbox() const;


private:
	// dispear after a while after death
	short deathTimer;
	
	std::vector<Cell> collisionObjects;

	sf::Sprite sprite;
	sf::Texture texture;
	sf::Texture normalTexture;
	sf::Texture squishedTexture;

	sf::SoundBuffer squishedBuffer;
	sf::Sound squishedSound;
};