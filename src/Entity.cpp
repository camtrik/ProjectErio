#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Global.h"
#include "MapManager.h"
#include "Erio.h"
#include "Entity.h"

Entity::Entity(const float p_x, const float p_y) :
	dead(0),
	horizontalSpeed(0),
	verticalSpeed(0),
	x(p_x),
	y(p_y)
{

}

bool Entity::getDead() const
{
	return dead;
}

void Entity::die(const unsigned char deathType)
{
	dead = true;
}

sf::FloatRect Entity::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}