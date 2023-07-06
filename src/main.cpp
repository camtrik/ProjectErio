#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Erio.h"
#include "Global.h"
#include "MapManager.h"
#include "Slime.h"
#include "Bomb.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Project Erio", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));

	/*-----------------BGM-----------------------*/
	sf::Music bgm;
	loadMusicFile("Resources/Audio/mainBGM.ogg", bgm);

	bgm.setLoop(true); // 设置背景音乐为循环播放
	bgm.play();


	/*-----------------Game Loop Objects-----------------------*/
	// Camera
	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	// lag time since last update
	std::chrono::microseconds lag(0);
	// time of last update
	std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();;

	sf::Event event;

	sf::Color backgroundColor = sf::Color(0, 219, 255);

	Erio erio;
	std::vector<std::shared_ptr<Entity>> enemies;
	MapManager mapManager;
	//converSketch(enemies, erio, mapManager);
	mapManager.converSketch(enemies, erio);
	unsigned viewX = 0;

	/*-----------------test-----------------------*/
	/*-----------------Fonts-----------------------*/
	sf::Font font;
	if (!font.loadFromFile("Resources/Fonts/slkscr.ttf"))
	{
		std::cout << "Error loading font\n";
	}
	sf::Text text;
	text.setFont(font);
	text.setString("*3");
	text.setCharacterSize(24); // 文字大小，以像素为单位
	text.setFillColor(sf::Color::Black); // 文字颜色
	text.setPosition(50, 50); // 文字位置

	Bomb bomb(300, 100);
	Slime slime(300, 100);

	while (window.isOpen())
	{
		std::chrono::microseconds deltaTime =
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previousTime);
		previousTime += deltaTime;
		lag += deltaTime;
		while (FRAME_DURATION <= lag) {
			// update game logic
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			// update the camera
			viewX = std::clamp<int>(round(erio.getX() - SCREEN_WIDTH / 2), 0, CELL_SIZE * mapManager.getMapSize() - SCREEN_WIDTH);
			mapManager.update();
			erio.update(viewX, mapManager);
			
			//bomb.update(viewX, mapManager, erio);

			for (auto& enemy : enemies) {
				enemy->update(viewX, mapManager, erio, enemies);
			}

			enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::shared_ptr<Entity>& enemy) { return enemy->getDead(); }), enemies.end());

			lag -= FRAME_DURATION;
		}
		if (FRAME_DURATION > lag) {
			// the camera
			// set the width of the view to be half of the screen width, the texture will be stretched twice
			view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

			window.setView(view);

			window.clear(backgroundColor);
			
			mapManager.drawMapBackground(viewX, window);
			erio.drawBombs(viewX, window);
			erio.draw(viewX, window);
			mapManager.drawMapBlocks(viewX, window);

		
			//bomb.draw(viewX, window);

			for (auto& enemy : enemies) {
				enemy->draw(viewX, window);
			}

			erio.displayMessage(viewX, window);
			window.display();
		}
	}
	return 0;
}
