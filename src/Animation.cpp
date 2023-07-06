#include "Animation.h"

#include <iostream>
Animation::Animation(const std::string& texturePath, const unsigned short frameWidth, const unsigned short animationSpeed)
	: frameWidth(frameWidth), animationSpeed(animationSpeed)
{
	if (!texture.loadFromFile(texturePath)) {
		std::cout << "Error loading texture: " << texturePath << std::endl;
	}
	sprite.setTexture(texture);
	totalFrame = texture.getSize().x / frameWidth;
}

void Animation::draw(sf::RenderWindow& window)
{
	sprite.setTexture(texture);
	if (!flip) {
		sprite.setTextureRect(sf::IntRect(frameWidth * currentFrame, 0, frameWidth, texture.getSize().y));
	}
	else {
		sprite.setTextureRect(sf::IntRect(frameWidth * (currentFrame + 1), 0, -frameWidth, texture.getSize().y));
	}
	window.draw(sprite);
}

void Animation::update()
{
	// when passed time is greater than animation speed, update the frame
	while (timePassed >= animationSpeed) {
		timePassed -= animationSpeed;
		currentFrame = (currentFrame + 1) % totalFrame;
	}
	timePassed++;
}
