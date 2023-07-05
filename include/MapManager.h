#pragma once

class Entity;
class Slime;
class Erio;

class MapManager
{
public: 
	MapManager();
	void drawMapBlocks(const unsigned viewX, sf::RenderWindow& window);
	void drawMapBackground(const unsigned viewX, sf::RenderWindow& window);
	void converSketch(std::vector<std::shared_ptr<Entity>>& enemies, Erio& erio);
	std::vector<unsigned char> mapCollisions(const std::vector<Cell>& cellsChecked, sf::FloatRect hitbox);

	void setMapSize(const unsigned short newSize);
	unsigned short getMapSize() const { return map.size(); }

private:
	sf::Image mapSketch;
	sf::Sprite cellSprite;
	sf::Texture mapTexture;

	Map map;
};