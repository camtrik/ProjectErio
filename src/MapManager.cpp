#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Animation.h"
#include "Global.h"
#include "MapManager.h"
#include "Erio.h"
#include "Entity.h"
#include "Slime.h"

MapManager::MapManager() :
	questionBlockAnimation("Resources/Images/QuestionBlock.png", CELL_SIZE, QUESTION_ANIMATION_SPEED),
	coinAnimation("Resources/Images/Coin.png", CELL_SIZE, COIN_ANIMATION_SPEED)
{
	// load map 
	mapSketch.loadFromFile("Resources/Images/LevelSketch0.png");
	mapTexture.loadFromFile("Resources/Images/Map.png");
	cellSprite.setTexture(mapTexture);
	setMapSize(mapSketch.getSize().x);

	// load audio
	loadAudioFile("Resources/Audio/explosion.wav", explosionSoundBuffer, explosionSound);
	loadAudioFile("Resources/Audio/coin.wav", coinSoundBuffer, coinSound);
}

void MapManager::update()
{
	// questionBlock coins update 
	for (auto& coin : questionBlockCoins) {
		coin.verticalSpeed += GRAVITY;
		coin.y += coin.verticalSpeed;
	}
	// delete coins
	questionBlockCoins.erase(std::remove_if(questionBlockCoins.begin(), questionBlockCoins.end(), [](const SimpleObject& coin) {
		return coin.verticalSpeed >= 0;
		}), questionBlockCoins.end());
	
	// explosion update
	for (auto& explosion : explosions) {
		explosion.animation.update();
	}
	// delete explosion
	explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](Explosion& explosion) {
		return explosion.animation.isFinished();
		}), explosions.end());
	

	questionBlockAnimation.update();
	coinAnimation.update();
}

// one cell = 16x16 pixels
void MapManager::drawMapBlocks(const unsigned viewX, sf::RenderWindow& window)
{
	// displayed cells wide
	unsigned short mapEnd = ceil((viewX + SCREEN_WIDTH) / static_cast<float>(CELL_SIZE));
	// displayed cells tall
	unsigned short mapHeight = floor(mapSketch.getSize().y / 3.f);
	// the start x of the current map
	unsigned short mapStart = floor(viewX / static_cast<float>(CELL_SIZE));

	bool drawStaticSprite = true;

	for (unsigned short x = mapStart; x < mapEnd; x++)
	{
		for (unsigned short y = 0; y < mapHeight; y++)
		{
			unsigned short spriteX = 0;
			unsigned short spriteY = 0;
			cellSprite.setPosition(CELL_SIZE * x, CELL_SIZE * y);
			// where to draw the block
			if (map[x][y] != Cell::Empty) {
				// if the part have animation
				if (map[x][y] == Cell::QuestionBlock) {
					questionBlockAnimation.setPosition(CELL_SIZE * x, CELL_SIZE * y);
					questionBlockAnimation.draw(window);

				}
				else if (map[x][y] == Cell::Coin) {
					coinAnimation.setPosition(CELL_SIZE * x, CELL_SIZE * y);
					coinAnimation.draw(window);
				}
				// if the part is static
				else {
					if (map[x][y] == Cell::Wall) {
						if (sf::Color(0, 0, 0) == mapSketch.getPixel(x, y))
							spriteX = 2;
						else
							spriteX = 3;
					}
					else if (map[x][y] == Cell::Brick) {
						spriteX = 0;
					}
					else if (map[x][y] == Cell::ActivatedQuestionBlock) {
						spriteX = 6;
						spriteY = 1;
					}
					else if (map[x][y] == Cell::Pipe) {
						if (sf::Color(0, 182, 0) == mapSketch.getPixel(x, y))
						{
							spriteY = 1;

							if (Cell::Pipe == map[x - 1][y])
							{
								spriteX = 11;
							}
							else
							{
								spriteX = 10;
							}
						}
						else if (sf::Color(0, 146, 0) == mapSketch.getPixel(x, y))
						{
							spriteY = 0;

							if (sf::Color(0, 146, 0) == mapSketch.getPixel(x - 1, y))
							{
								spriteX = 11;
							}
							else if (sf::Color(0, 146, 0) == mapSketch.getPixel(1 + x, y))
							{
								spriteX = 10;
							}
							else
							{
								spriteX = 10;

								if (sf::Color(0, 146, 0) == mapSketch.getPixel(x, y - 1))
								{
									spriteY = 3;
								}
								else
								{
									spriteY = 2;
								}
							}
						}
						else if (sf::Color(0, 219, 0) == mapSketch.getPixel(x, y))
						{
							if (sf::Color(0, 182, 0) == mapSketch.getPixel(1 + x, y))
							{
								spriteX = 12;
							}
							else
							{
								spriteX = 11;
							}

							if (sf::Color(0, 219, 0) == mapSketch.getPixel(x, y - 1))
							{
								spriteY = 3;
							}
							else
							{
								spriteY = 2;
							}
						}
					}
					//else if (map[x][y] == Cell::)
					// read texture block from the png file
					if (drawStaticSprite) {
						cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * spriteX, CELL_SIZE * spriteY, CELL_SIZE, CELL_SIZE));
						window.draw(cellSprite);
					}
				}
			}
			
			/*---------------test part-------------------*/
			//if (mapSketch.getPixel(x, y) == sf::Color(0, 0, 0) || mapSketch.getPixel(x,y) == sf::Color(146, 73, 0))
			//{
			//	cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * 2, CELL_SIZE * 0, CELL_SIZE, CELL_SIZE));
			//	window.draw(cellSprite);
			//	map[x][y] = Cell::Wall;
			//}

			//if (y > mapHeight - 5 && (x == 3 || x == 10) || y == mapHeight - 5 && (x > 7 && x <= 10)) {
			//	cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * 2, CELL_SIZE * 0, CELL_SIZE, CELL_SIZE));
			//	window.draw(cellSprite);
			//	map[x][y] = Cell::Wall;
			//}

			/*---------------test part-------------------*/
		}
	}
	// draw the explosion on all other things
	for (auto& explosion : explosions) {
		explosion.animation.setPosition(explosion.x, explosion.y);
		explosion.animation.draw(window);
		//explosion.timer--;
	}
}

void MapManager::drawMapBackground(const unsigned viewX, sf::RenderWindow& window)
{
	// displayed cells wide
	unsigned short mapEnd = ceil((viewX + SCREEN_WIDTH) / static_cast<float>(CELL_SIZE));
	// displayed cells tall
	unsigned short mapHeight = floor(mapSketch.getSize().y / 3.f);
	// the start x of the current map
	unsigned short mapStart = floor(viewX / static_cast<float>(CELL_SIZE));

	// draw the coin come from the question block
	for (const auto& coin : questionBlockCoins) {
		coinAnimation.setPosition(coin.x, coin.y);
		coinAnimation.draw(window);
	}

	for (unsigned short x = mapStart; x < mapEnd; x++)
	{
		for (unsigned short y = 0; y < mapHeight; y++)
		{
			unsigned short spriteX = 0;
			unsigned short spriteY = 0;
			cellSprite.setPosition(CELL_SIZE * x, CELL_SIZE * y);
			sf::Color pixel = mapSketch.getPixel(x, y + 2 * mapHeight);
			sf::Color pixel_down = sf::Color(0, 0, 0, 0);
			sf::Color pixel_left = sf::Color(0, 0, 0, 0);
			sf::Color pixel_right = sf::Color(0, 0, 0, 0);
			sf::Color pixel_up = sf::Color(0, 0, 0, 0);

			if (255 == pixel.a)
			{
				if (0 < x)
				{
					pixel_left = mapSketch.getPixel(x - 1, y + 2 * mapHeight);
				}

				if (0 < y)
				{
					pixel_up = mapSketch.getPixel(x, y + 2 * mapHeight - 1);
				}

				if (x < mapSketch.getSize().x - 1)
				{
					pixel_right = mapSketch.getPixel(1 + x, y + 2 * mapHeight);
				}

				if (y < mapHeight - 1)
				{
					pixel_down = mapSketch.getPixel(x, 1 + y + 2 * mapHeight);
				}
				// cloud
				if (sf::Color(255, 255, 255) == pixel)
				{
					spriteX = 8;

					if (sf::Color(255, 255, 255) == pixel_up)
					{
						spriteY = 1;
					}

					if (sf::Color(255, 255, 255) == pixel_left)
					{
						if (sf::Color(255, 255, 255) != pixel_right)
						{
							spriteX = 9;
						}
					}
					else if (sf::Color(255, 255, 255) == pixel_right)
					{
						spriteX = 7;
					}
				}
				// grass
				else if (sf::Color(146, 219, 0) == pixel)
				{
					spriteX = 5;
				}
				// 深黄色
				else if (sf::Color(146, 146, 0) == pixel)
				{
					spriteX = 4;
				}
				// 绿黄色
				else if (sf::Color(146, 182, 0) == pixel)
				{
					spriteX = 6;
				}
				// 深绿色
				else if (sf::Color(0, 73, 0) == pixel)
				{
					spriteY = 1;

					if (sf::Color(0, 109, 0) == pixel_left)
					{
						if (sf::Color(0, 109, 0) != pixel_right)
						{
							spriteX = 2;
						}
					}
					else if (sf::Color(0, 109, 0) == pixel_right)
					{
						spriteX = 1;
					}
				}
				// 更深的绿色
				else if (sf::Color(0, 109, 0) == pixel)
				{
					spriteY = 1;

					if (sf::Color(0, 73, 0) == pixel_left)
					{
						spriteX = 3;
					}
					else if (sf::Color(0, 73, 0) == pixel_right)
					{
						spriteX = 5;
					}
					else
					{
						spriteX = 4;
					}
				}
				// 浅绿色
				else if (sf::Color(109, 255, 85) == pixel)
				{
					spriteX = 12;

					if (sf::Color(109, 255, 85) == pixel_up)
					{
						spriteY = 1;
					}
				}

				cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * spriteX, CELL_SIZE * spriteY, CELL_SIZE, CELL_SIZE));

				window.draw(cellSprite);
			}
		}
	}

}

std::vector<unsigned char> MapManager::mapCollisions(const std::vector<Cell>& cellsChecked, sf::FloatRect hitbox) const
{
	std::vector<unsigned char> output;

	for (short a = floor(hitbox.top / CELL_SIZE); a <= floor((ceil(hitbox.height + hitbox.top) - 1) / CELL_SIZE); a++)
	{
		output.push_back(0);

		for (short b = floor(hitbox.left / CELL_SIZE); b <= floor((ceil(hitbox.left + hitbox.width) - 1) / CELL_SIZE); b++)
		{
			if (0 <= b && b < map.size())
			{
				if (0 <= a && a < map[0].size())
				{
					if (cellsChecked.end() != std::find(cellsChecked.begin(), cellsChecked.end(), map[b][a]))
					{
						//We're gonna return a vector of numbers. Each number is a binary representation of collisions in a single row.
						output[a - floor(hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(hitbox.left + hitbox.width) - 1) / CELL_SIZE) - b);
					}
				}
			}
			//We're assuming that the map borders have walls.
			else if (cellsChecked.end() != std::find(cellsChecked.begin(), cellsChecked.end(), Cell::Wall))
			{
				output[a - floor(hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(hitbox.left + hitbox.width) - 1) / CELL_SIZE) - b);
			}
		}
	}

	return output;
}

std::vector<unsigned char> MapManager::mapCollisions(const std::vector<Cell>& cellsChecked, sf::FloatRect hitbox, std::vector<sf::Vector2i>& cellsCollied) const
{
	std::vector<unsigned char> output;

	for (short a = floor(hitbox.top / CELL_SIZE); a <= floor((ceil(hitbox.height + hitbox.top) - 1) / CELL_SIZE); a++)
	{
		output.push_back(0);

		for (short b = floor(hitbox.left / CELL_SIZE); b <= floor((ceil(hitbox.left + hitbox.width) - 1) / CELL_SIZE); b++)
		{
			if (0 <= b && b < map.size())
			{
				if (0 <= a && a < map[0].size())
				{
					if (cellsChecked.end() != std::find(cellsChecked.begin(), cellsChecked.end(), map[b][a]))
					{
						// return the position of cells that collied
						cellsCollied.push_back(sf::Vector2i(b, a));
						//We're gonna return a vector of numbers. Each number is a binary representation of collisions in a single row.
						output[a - floor(hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(hitbox.left + hitbox.width) - 1) / CELL_SIZE) - b);
					}
				}
			}
			//We're assuming that the map borders have walls.
			else if (cellsChecked.end() != std::find(cellsChecked.begin(), cellsChecked.end(), Cell::Wall))
			{
				output[a - floor(hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(hitbox.left + hitbox.width) - 1) / CELL_SIZE) - b);
			}
		}
	}

	return output;
}


void MapManager::converSketch(std::vector<std::shared_ptr<Entity>>& enemies, Erio& erio)
{
	unsigned short mapHeight = floor(mapSketch.getSize().y / 3.f);
	unsigned short mapWidth = mapSketch.getSize().x;
	std::cout << map[0].size() << std::endl;
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			sf::Color pixel = mapSketch.getPixel(x, y);
			if (sf::Color(182, 73, 0) == pixel)
			{
				map[x][y] = Cell::Brick;
			}
			else if (sf::Color(255, 255, 0) == pixel)
			{
				map[x][y] = Cell::Coin;
			}
			else if (sf::Color(0, 146, 0) == pixel || sf::Color(0, 182, 0) == pixel || sf::Color(0, 219, 0) == pixel)
			{
				//Multiple colors, because we need to know which part of the pipe we need to draw.
				map[x][y] = Cell::Pipe;
			}
			else if (sf::Color(255, 73, 85) == pixel || sf::Color(255, 146, 85) == pixel)
			{
				map[x][y] = Cell::QuestionBlock;
			}
			else if (sf::Color(0, 0, 0) == pixel || sf::Color(146, 73, 0) == pixel)
			{
				map[x][y] = Cell::Wall;
			}
			else
			{
				map[x][y] = Cell::Empty;

				//if (sf::Color(0, 255, 255) == pixel)
				//{
				//	i_level_finish = a;
				//}
			}
		}
	}

	for (int x = 0; x < mapWidth; x++) {
		for (int y = mapHeight; y < 2 * mapHeight; y++) {
			sf::Color pixel = mapSketch.getPixel(x, y);
			if (pixel == sf::Color(182, 73, 0)) {
				enemies.push_back(std::make_shared<Slime>(CELL_SIZE * x, CELL_SIZE * (y - mapHeight)));
			}
			else if (pixel == sf::Color(0, 219, 0)) {
				// other type of enemies

			}
		}
	}
}

// set cell as 
void MapManager::setMapCell(const unsigned short p_x, const unsigned short p_y, const Cell& cell)
{
	map[p_x][p_y] = cell;
}

// resize map width size, the height are always the same 
void MapManager::setMapSize(const unsigned short newSize)
{
	map.clear();
	map.resize(newSize);
}

sf::Color MapManager::getMapPixel(const unsigned short p_x, const unsigned short p_y) const
{
	return mapSketch.getPixel(p_x, p_y);
}

void MapManager::addCoin(const short p_x, const short p_y)
{
	questionBlockCoins.push_back(SimpleObject(p_x, p_y, 0, -COIN_JUMP_SPEED));
}

void MapManager::addExplosion(const float p_x, const float p_y)
{
	explosions.push_back(Explosion(p_x, p_y));
	explosionSound.play();
}

void MapManager::playSound(const unsigned short soundType)
{
	switch(soundType) {
	case 0:
		// play coin sound
		coinSound.play();
		break;
	}
}