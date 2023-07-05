#pragma once
#include <SFML/Graphics.hpp>
#include <vector>"
#include "Global.h"


class MapManager
{
public: 
	MapManager();
	void drawMapBlocks(const unsigned viewX, sf::RenderWindow& window);
	std::vector<unsigned char> mapCollisions(const std::vector<Cell>& cellsChecked, sf::FloatRect hitbox);

	void setMapSize(const unsigned short newSize);
	unsigned short getMapSize() const { return map.size(); }

private:
	sf::Image mapSketch;
	sf::Sprite cellSprite;
	sf::Texture mapTexture;

	Map map;
};