#pragma once
#include <SFML/Graphics.hpp>
#include "MapManager.h"
#include "Global.h"
class Erio
{
public:
	Erio();
	void draw(sf::RenderWindow &window);
	void update(MapManager& mapManager);
	float getX() const { return x; }

	sf::FloatRect getHitbox() const;
private:
	float x;
	float y;
	float verticalSpeed{0};
	float horizontalSpeed{0};
	int jumpTimer{ 0 };

	sf::Sprite sprite;
	sf::Texture texture;
};