#include <array>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Global.h"
#include "MapManager.h"
#include "Erio.h"
#include "Entity.h"
#include "Slime.h"
#include <iostream>

Slime::Slime(const float p_x, const float p_y) :
	Entity(p_x, p_y), collisionObjects{ Cell::Wall, Cell::Brick, Cell::QuestionBlock, Cell::Pipe },
	deathTimer(-1)
{
	texture.loadFromFile("Resources/Images/slime.png");
	sprite.setTexture(texture);
	horizontalSpeed = -SLIME_SPEED;

}

void Slime::die(const unsigned char deathType)
{
	switch (deathType) {
	case 0:
		dead = true;
		break;
	case 1:
		// get crashed by the player
		deathTimer = SLIME_DEATH_DURATION;
		texture.loadFromFile("Resources/Images/slimeDead.png");
		break;
	}
}

void Slime::draw(const unsigned viewX, sf::RenderWindow& window)
{
	// if not in the view, then don't draw
	if (!(x > static_cast<int>(viewX) - UPDATE_AREA && x < viewX + SCREEN_WIDTH + UPDATE_AREA && y > -CELL_SIZE && y < SCREEN_HEIGHT)) {
		return;
	}
	
	sprite.setPosition(x, y);
	// normal
	window.draw(sprite);
}

void Slime::update(const unsigned viewX, MapManager& mapManager, Erio& erio, const std::vector<std::shared_ptr<Entity>>& enemies)
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
	/*----------------------------moving------------------------------*/
	

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

	// slimes collisions with other enemies
	for (const auto& enemy : enemies) {
		if (enemy.get() != this && enemy->getHitbox().intersects(getHitbox())) {
			horizontalSpeed = -horizontalSpeed;
			break;
		}
	}

	// slimes collisions with the player
	if (erio.getHitbox().intersects(getHitbox()) && deathTimer < 0) {

		if (erio.getVerticalSpeed() > 0) {
			die(1);
			erio.setVerticalSpeed(-ERIO_JUMP * 0.5);
		}
		else {
			erio.die(1);
		}
		
	}
	
	// death
	if (deathTimer > 0) {
		horizontalSpeed = 0;
		deathTimer--;
	}
	if (deathTimer == 0) {
		dead = true;
	}
	
	/*----------------------------animation------------------------------*/

}

sf::FloatRect Slime::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}
