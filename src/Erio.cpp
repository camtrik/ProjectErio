#include "Erio.h"

#include <iostream>
Erio::Erio()
{
	x = 50;
	y = 50;
	texture.loadFromFile("Resources/Images/MarioIdle.png");
	sprite.setTexture(texture);
}

void Erio::draw(sf::RenderWindow& window)
{
	sprite.setPosition(x, y);
	window.draw(sprite);
}

void Erio::update(MapManager& mapManager)
{
	sf::FloatRect hitbox = getHitbox();

	std::vector<unsigned char> collisions;

	bool moving = false; // if the player is moving (if the key left or right is pressed)
	
	// moving 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		horizontalSpeed = std::max(horizontalSpeed - ERIO_ACCELERATE, -ERIO_SPEED);
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		horizontalSpeed = std::min(horizontalSpeed + ERIO_ACCELERATE, ERIO_SPEED);
		moving = true;
	}
	// jumping
	hitbox = getHitbox();
	// to detect the collision to the floor
	hitbox.top += 1;
	collisions = mapManager.mapCollisions({ Cell::Wall }, hitbox);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		// only if the player is on the ground
		if (verticalSpeed == 0 && 0 == std::all_of(collisions.begin(), collisions.end(), [](const unsigned char value) { return value == 0; })) {
			verticalSpeed = -ERIO_JUMP;
			jumpTimer = ERIO_JUMP_TIMER;
		}
		else if (jumpTimer > 0) {
			verticalSpeed = -ERIO_JUMP;
			jumpTimer--;
		}
	}
	verticalSpeed += GRAVITY;
	

	// if not moving, slow down the speed
	if (!moving) {
		if (horizontalSpeed > 0) {
			horizontalSpeed = std::max(0.f, horizontalSpeed - ERIO_ACCELERATE);
		}
		else if (horizontalSpeed < 0) {
			horizontalSpeed = std::min(0.f, horizontalSpeed + ERIO_ACCELERATE);
		}
	}
	
	

	// horizental collision
	hitbox = getHitbox();
	hitbox.left += horizontalSpeed;
	
	collisions = mapManager.mapCollisions({ Cell::Wall }, hitbox);
	if (std::all_of(collisions.begin(), collisions.end(), [](const unsigned char value) { return value == 0; })) {
		x += horizontalSpeed;
	}
	else {
		if (horizontalSpeed > 0) {
			x = (floor(hitbox.left / CELL_SIZE)) * CELL_SIZE;
		}
		else if (horizontalSpeed < 0) {
			x = (floor(hitbox.left / CELL_SIZE) + 1) * CELL_SIZE;
		}
		horizontalSpeed = 0;
	}
	// vertical collision
	
	hitbox = getHitbox();
	hitbox.top += verticalSpeed;

	collisions = mapManager.mapCollisions({ Cell::Wall }, hitbox);
	std::cout << collisions[0] << std::endl;
	// if all collisions are 0, then no collisions
	if (std::all_of(collisions.begin(), collisions.end(), [](const unsigned char value) { return value == 0; })) {
		y += verticalSpeed;
	}
	else {
		if (verticalSpeed > 0) {
			y = (floor(hitbox.top / CELL_SIZE)) * CELL_SIZE;
		}
		else if (verticalSpeed < 0) {
			y = (floor(hitbox.top / CELL_SIZE) + 1) * CELL_SIZE;
		}
		verticalSpeed = 0;
	}
}

sf::FloatRect Erio::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}