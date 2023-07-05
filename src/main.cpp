#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>


#include "Erio.h"
#include "Global.h"
#include "MapManager.h"

int main()
{
	//sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH * SCREEN_RESIZED, SCREEN_HEIGHT * SCREEN_RESIZED), "ProjectErio");
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Super Mario Bros", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));

	// ÉãÏñ»ú
	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	// lag time since last update
	std::chrono::microseconds lag(0);
	// time of last update
	std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();;

	sf::Event event;

	sf::Color backgroundColor = sf::Color(0, 219, 255);

	Erio erio;

	MapManager mapManager;

	unsigned viewX = 0;

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
			erio.update(mapManager);
			lag -= FRAME_DURATION;
			
		}
		if (FRAME_DURATION > lag) {
			// the camera
			// set the width of the view to be half of the screen width, the texture will be stretched twice
			view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

			window.setView(view);

			window.clear(backgroundColor);
			
			mapManager.drawMapBlocks(viewX, window);

			erio.draw(window);
			
			window.display();
		}
	}
	return 0;
}
