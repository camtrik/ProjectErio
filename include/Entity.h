#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
class Entity
{
public:
	Entity(const float p_x, const float p_y);
	virtual void draw(const unsigned viewX, sf::RenderWindow& window) = 0;
	virtual void update(const unsigned viewX, MapManager& mapManager, Erio& erio, const std::vector<std::shared_ptr<Entity>>& enemies) = 0;
	virtual bool getDead() const;
	virtual void die(const unsigned char deathType);
	sf::FloatRect getHitbox() const;

protected:
	bool dead;
	float x;
	float y;
	float verticalSpeed;
	float horizontalSpeed;
};