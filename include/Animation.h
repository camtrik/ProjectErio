#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

#include "Global.h"

class Animation
{
public:
	Animation() {};
	Animation(const std::string& texturePath, const unsigned short frameWidth, const unsigned short animationSpeed);
	void draw(sf::RenderWindow& window);
	void update();
	void setPosition(const float p_x, const float p_y) { sprite.setPosition(p_x, p_y); }
	void setSpeed(int p_speed) { animationSpeed = std::max<unsigned short>(1, p_speed);}
	void setFlip(const bool p_flip) { flip = p_flip; }

private:
	unsigned short totalFrame;
	unsigned short currentFrame{ 0 };
	unsigned short animationSpeed;  
	unsigned short timePassed; 
	unsigned short frameWidth;
	

	sf::Sprite sprite;
	sf::Texture texture;

	bool flip{ false };


};