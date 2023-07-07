#pragma once
#include "Explosion.h"

class Entity;
class Slime;
class Erio;
class Bomb;

class MapManager
{
public: 
	MapManager();
	void update();
	void drawMapBlocks(const unsigned viewX, sf::RenderWindow& window);
	void drawMapBackground(const unsigned viewX, sf::RenderWindow& window);
	void converSketch(std::vector<std::shared_ptr<Entity>>& enemies, Erio& erio);
	std::vector<unsigned char> mapCollisions(const std::vector<Cell>& cellsChecked, sf::FloatRect hitbox) const;
	std::vector<unsigned char> mapCollisions(const std::vector<Cell>& cellsChecked, sf::FloatRect hitbox, std::vector<sf::Vector2i>& cellsCollied) const;
	void setMapCell(const unsigned short p_x, const unsigned short p_y, const Cell& cell);
	void setMapSize(const unsigned short newSize);
	unsigned short getMapSize() const { return map.size(); }
	sf::Color getMapPixel(const unsigned short p_x, const unsigned short p_y) const;
	void addCoin(const short p_x, const short p_y);
	void addExplosion(const float p_x, const float p_y);
	std::vector<Explosion> getExplosions() const { return explosions; }
	void playSound(const unsigned short soundType);

private:
	sf::Image mapSketch;
	sf::Sprite cellSprite;
	sf::Texture mapTexture;

	Animation questionBlockAnimation;
	Animation coinAnimation;
	Animation portalAnimation;

	std::vector<SimpleObject> questionBlockCoins;
	std::vector<Explosion> explosions;

	sf::SoundBuffer coinSoundBuffer;
	sf::Sound coinSound;
	sf::SoundBuffer explosionSoundBuffer;
	sf::Sound explosionSound;

	Map map;
};