#include "MapManager.h"

#include <iostream>

MapManager::MapManager()
{
	mapSketch.loadFromFile("Resources/Images/LevelSketch0.png");
	mapTexture.loadFromFile("Resources/Images/Map.png");
	cellSprite.setTexture(mapTexture);

	setMapSize(mapSketch.getSize().x);
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

	for (unsigned short x = mapStart; x < mapEnd; x++)
	{
		for (unsigned short y = 0; y < mapHeight; y++)
		{
			unsigned short spriteX = 0;
			unsigned short spriteY = 0;
			cellSprite.setPosition(CELL_SIZE * x, CELL_SIZE * y);
			if (mapSketch.getPixel(x, y) == sf::Color(0, 0, 0) || mapSketch.getPixel(x,y) == sf::Color(146, 73, 0))
			{
				cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * 2, CELL_SIZE * 0, CELL_SIZE, CELL_SIZE));
				window.draw(cellSprite);
				map[x][y] = Cell::Wall;
			}
			/*---------------test part-------------------*/ 
			if (y > mapHeight - 5 && (x == 3 || x == 10) || y == mapHeight - 5 && (x > 7 && x <= 10)) {
				cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * 2, CELL_SIZE * 0, CELL_SIZE, CELL_SIZE));
				window.draw(cellSprite);
				map[x][y] = Cell::Wall;
			}

			/*---------------test part-------------------*/
		}
	}
}


std::vector<unsigned char> MapManager::mapCollisions(const std::vector<Cell>& cellsChecked, sf::FloatRect hitbox)
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

// resize map width size, the height are always the same 
void MapManager::setMapSize(const unsigned short newSize)
{
	map.clear();
	map.resize(newSize);
}

