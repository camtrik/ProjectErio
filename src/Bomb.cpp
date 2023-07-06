#include <array>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Global.h"
#include "MapManager.h"
#include "Erio.h"
#include "Entity.h"
#include "Bomb.h"
#include <iostream>

Bomb::Bomb(const float p_x, const float p_y) :
	Entity(p_x, p_y),
	startY(p_y),
	collisionObjects{ Cell::Wall, Cell::Brick, Cell::QuestionBlock, Cell::Pipe },
	deathTimer(-1)
{	
	if (!texture.loadFromFile("Resources/Images/bomb.png")) {
		std::cout << "Error loading bomb.png" << std::endl;
	}

	horizontalSpeed = BOMB_SPEED;
	sprite.setTexture(texture);
}

void Bomb::die(const unsigned char deathType)
{
	switch (deathType) {
	case 0:
		dead = true;
		break;
	case 1:
		// get crashed by the player

		break;
	}
}

void Bomb::draw(const unsigned viewX, sf::RenderWindow& window)
{
	// if not in the view, then don't draw
	if (!(x > static_cast<int>(viewX) - UPDATE_AREA && x < viewX + SCREEN_WIDTH + UPDATE_AREA && y > -CELL_SIZE && y < SCREEN_HEIGHT)) {
		return;
	}

	sprite.setPosition(x, y);
	sprite.setTexture(texture);
	// normal
	window.draw(sprite);
}

void Bomb::update(const unsigned viewX, MapManager& mapManager, Erio& erio)
{
	// if fall out of the screen, then die
	if (y >= SCREEN_HEIGHT) {
		die(0);
	}
	// if not in the view, then don't update
	if (!(x > static_cast<int>(viewX) - UPDATE_AREA && x < viewX + SCREEN_WIDTH + UPDATE_AREA && y > -CELL_SIZE && y < SCREEN_HEIGHT)) {
		return;
	}

	sf::FloatRect hitbox;
	std::vector<unsigned char> collisions;

	if (y > startY - CELL_SIZE) {
		verticalSpeed = -BOMB_SPEED; 
		y += verticalSpeed;
		if (y <= startY - CELL_SIZE) {
			startY = 2 * SCREEN_HEIGHT;
		}
		return;
	}

	/*----------------------------collisions------------------------------*/
	// horizental collision
	verticalSpeed += GRAVITY;
	hitbox = getHitbox();
	hitbox.left += horizontalSpeed;

	collisions = mapManager.mapCollisions(collisionObjects, hitbox);
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
		horizontalSpeed = -horizontalSpeed;
	}
	// vertical collision
	hitbox = getHitbox();
	hitbox.top += verticalSpeed;

	collisions = mapManager.mapCollisions(collisionObjects, hitbox);
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

	// Bombs collisions with the player
	if (!erio.getDead() && erio.getHitbox().intersects(getHitbox()) && deathTimer < 0) {
		erio.getBomb();
		die(0);
	}

	/*----------------------------death------------------------------*/
	if (deathTimer > 0) {
		horizontalSpeed = 0;
		deathTimer--;
	}
	if (deathTimer == 0) {
		dead = true;
	}
	
}

sf::FloatRect Bomb::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}
