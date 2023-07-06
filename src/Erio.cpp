#include <algorithm>
#include "Animation.h"
#include "Global.h"
#include "MapManager.h"
#include "Erio.h"

#include <iostream>
Erio::Erio() :
	verticalSpeed(0),
	horizontalSpeed(0),
	jumpTimer(0),
	flip(false),
	dead(false),
	deathTimer(-1),
	collisionObjects{ Cell::Wall, Cell::Brick, Cell::QuestionBlock, Cell::Pipe, Cell::ActivatedQuestionBlock },
	walkAnimation("Resources/Images/MarioWalk.png", CELL_SIZE, ERIO_ANIMATION_SPEED)
{
	x = 50;
	y = 50;
	texture.loadFromFile("Resources/Images/MarioIdle.png");
	idleTexture.loadFromFile("Resources/Images/MarioIdle.png");
	jumpTexture.loadFromFile("Resources/Images/MarioJump.png");
	deathTexture.loadFromFile("Resources/Images/MarioDeath.png");
	sprite.setTexture(texture);

	loadAudioFile("Resources/Audio/jump.wav", jumpSoundBuffer, jumpSound);
	loadMusicFile("Resources/Audio/death.wav", deadSound);
}

void Erio::draw(sf::RenderWindow& window)
{
	sprite.setPosition(x, y);
	if (dead) {
		texture = deathTexture;
		sprite.setTexture(texture);
		window.draw(sprite);
		return;
	}

	bool drawStaticSprite = true;
	// normal
	texture = idleTexture;

	if (verticalSpeed != 0) {
		texture = jumpTexture;
	}

	if (horizontalSpeed != 0 && verticalSpeed == 0) {
		drawStaticSprite = false;
		walkAnimation.setFlip(flip);
		walkAnimation.setPosition(x, y);
		walkAnimation.draw(window);
	}
	
	if (drawStaticSprite) {
		if (flip) {
			sprite.setTextureRect(sf::IntRect(texture.getSize().x, 0, -static_cast<int>(texture.getSize().x), texture.getSize().y));
		}
		else {
			sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
		}
		window.draw(sprite);
	}
}

void Erio::update(MapManager& mapManager)
{
	/*----------------------------death------------------------------*/
	if (dead) {
		deathTimer = std::max(0, deathTimer - 1);
		if (deathTimer == 1) {
			verticalSpeed = -ERIO_JUMP;
		}
		else if (deathTimer == 0) {
			y += verticalSpeed;
			verticalSpeed += GRAVITY;
		}
		return;
	}

	sf::FloatRect hitbox = getHitbox();

	std::vector<unsigned char> collisions;

	std::vector<sf::Vector2i> cellsPosition;

	bool moving = false; // if the player is moving (if the key left or right is pressed)
	
	/*----------------------------moving------------------------------*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		horizontalSpeed = std::max(horizontalSpeed - ERIO_ACCELERATE, -ERIO_SPEED);
		flip = true;
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		horizontalSpeed = std::min(horizontalSpeed + ERIO_ACCELERATE, ERIO_SPEED);
		flip = false;
		moving = true;
	}
	// if not moving, slow down the speed
	if (!moving) {
		if (horizontalSpeed > 0) {
			horizontalSpeed = std::max(0.f, horizontalSpeed - ERIO_ACCELERATE);
		}
		else if (horizontalSpeed < 0) {
			horizontalSpeed = std::min(0.f, horizontalSpeed + ERIO_ACCELERATE);
		}
	}

	/*----------------------------jumping------------------------------*/
	hitbox = getHitbox();
	// to detect the collision to the floor
	hitbox.top += 1;
	collisions = mapManager.mapCollisions(collisionObjects, hitbox);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		// only if the player is on the ground
		if (verticalSpeed == 0 && 0 == std::all_of(collisions.begin(), collisions.end(), [](const unsigned char value) { return value == 0; })) {
			jumpSound.play();
			verticalSpeed = -ERIO_JUMP;
			jumpTimer = ERIO_JUMP_TIMER;
		}
		else if (jumpTimer > 0) {
			verticalSpeed = -ERIO_JUMP;
			jumpTimer--;
		}
	}
	verticalSpeed += GRAVITY;
	
	/*----------------------------collisions------------------------------*/
	// horizental collision
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
		horizontalSpeed = 0;
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
			// colision with question blocks
			auto questionCollisions = mapManager.mapCollisions({Cell::QuestionBlock}, hitbox, cellsPosition);
			if (std::all_of(questionCollisions.begin(), questionCollisions.end(), [](const unsigned char value) { return value == 0; }) == 0) {
				for (auto& pos : cellsPosition) {
					mapManager.setMapCell(pos.x, pos.y, Cell::ActivatedQuestionBlock);
					if (sf::Color(255, 73, 85) == mapManager.getMapPixel(pos.x, pos.y))
					{
						// add a mushroom / other things
					}
					else
					{
						// add a coin 
						mapManager.addCoin(pos.x, pos.y);
						mapManager.playSound(0);
					}
				}
				
			}

			y = (floor(hitbox.top / CELL_SIZE) + 1) * CELL_SIZE;
		}
		verticalSpeed = 0;
	}

	/*----------------------------animation------------------------------*/
	walkAnimation.setSpeed((ERIO_SPEED * ERIO_ANIMATION_SPEED) / abs(horizontalSpeed));
	walkAnimation.update();

	/*----------------------------death------------------------------*/
	if (y > SCREEN_HEIGHT - CELL_SIZE) {
		die(1);
	}
}

void Erio::die(const unsigned short deathType)
{
	dead = true;
	deadSound.play();
	deathTimer = ERIO_DEATH_DURATION;
}


sf::FloatRect Erio::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}
