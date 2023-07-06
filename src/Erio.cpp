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
	bombCount(0),
	coinCount(0),
	killEnemyScore(0),
	score(0),
	collisionObjects{ Cell::Wall, Cell::Brick, Cell::QuestionBlock, Cell::Pipe, Cell::ActivatedQuestionBlock },
	walkAnimation("Resources/Images/MarioWalk.png", CELL_SIZE, ERIO_ANIMATION_SPEED)
{
	x = 50;
	y = 50;
	/*----------------------------Erio Texture------------------------------*/
	texture.loadFromFile("Resources/Images/MarioIdle.png");
	idleTexture.loadFromFile("Resources/Images/MarioIdle.png");
	jumpTexture.loadFromFile("Resources/Images/MarioJump.png");
	deathTexture.loadFromFile("Resources/Images/MarioDeath.png");
	sprite.setTexture(texture);

	/*----------------------------Message------------------------------*/
	font.loadFromFile("Resources/Fonts/slkscr.ttf");
	bombMessageText = sf::Text(": " + std::to_string(bombCount), font, HALF_CELL_SIZE);
	bombMessageText.setFillColor(sf::Color::Black);
	bombMessageText.setPosition(BOMB_MESSAGE_START_X + HALF_CELL_SIZE, BOMB_MESSAGE_START_Y - TOLERANCE_DIS);
	bombMessageTexture.loadFromFile("Resources/Images/bomb.png");
	bombMessageSprite.setTexture(bombMessageTexture);
	bombMessageSprite.setScale(0.5f, 0.5f);
	
	coinMessageText = sf::Text(": " + std::to_string(coinCount), font, HALF_CELL_SIZE);
	coinMessageText.setFillColor(sf::Color::Black);
	coinMessageText.setPosition(COIN_MESSAGE_START_X + HALF_CELL_SIZE, COIN_MESSAGE_START_Y - TOLERANCE_DIS);
	coinMessageTexture.loadFromFile("Resources/Images/oneCoin.png");
	coinMessageSprite.setTexture(coinMessageTexture);
	coinMessageSprite.setScale(0.5f, 0.5f);

	scoreText = sf::Text("SCORE: " + std::to_string(score), font, HALF_CELL_SIZE);
	scoreText.setFillColor(sf::Color::Black);
	scoreText.setPosition(SCORE_START_X, SCORE_START_Y - TOLERANCE_DIS);

	/*----------------------------Sound------------------------------*/
	loadAudioFile("Resources/Audio/jump.wav", jumpSoundBuffer, jumpSound);
	loadMusicFile("Resources/Audio/death.wav", deadSound);
}

void Erio::draw(unsigned viewX, sf::RenderWindow& window)
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

void Erio::update(unsigned viewX, MapManager& mapManager)
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
						addBombToMap(pos.x, pos.y);
					}
					else
					{
						// add a coin 
						coinCount++;
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

	/*----------------------------bombs update------------------------------*/
	for (auto& bomb : bombs) {
		bomb.update(viewX, mapManager, *this);
	}
	bombs.erase(std::remove_if(bombs.begin(), bombs.end(), [](const Bomb& bomb) { return bomb.getDead(); }), bombs.end());
	
	bombCount = bombsHave.size();

	/*-----------------------------Score-----------------------------------*/
	score = coinCount * 100 + killEnemyScore;
	
}

void Erio::die(const unsigned short deathType)
{
	dead = true;
	deadSound.play();
	deathTimer = ERIO_DEATH_DURATION;
}

void Erio::addBombToMap(const unsigned short p_x, const unsigned short p_y)
{
	bombs.push_back(Bomb(p_x * CELL_SIZE, p_y * CELL_SIZE));
}


void Erio::drawBombs(unsigned viewX, sf::RenderWindow& window)
{
	for (auto& bomb : bombs) {
		bomb.draw(viewX, window);
	}
}

// get one bomb, this is called when the player intersect the bomb, in the bomb.update()
void Erio::getBomb()
{
	bombsHave.push(Bomb(0, 0));
}

void Erio::displayMessage(unsigned viewX, sf::RenderWindow& window) 
{
	/*----------------------------bomb Message------------------------------*/
	bombMessageSprite.setPosition(viewX + BOMB_MESSAGE_START_X, BOMB_MESSAGE_START_Y);
	bombMessageText = sf::Text(": " + std::to_string(bombCount), font, HALF_CELL_SIZE);
	bombMessageText.setFillColor(sf::Color::Black);
	bombMessageText.setPosition(viewX + BOMB_MESSAGE_START_X + HALF_CELL_SIZE + TOLERANCE_DIS, BOMB_MESSAGE_START_Y - TOLERANCE_DIS);

	window.draw(bombMessageSprite);
	window.draw(bombMessageText);
	/*----------------------------coin Message------------------------------*/
	coinMessageSprite.setPosition(viewX + COIN_MESSAGE_START_X, COIN_MESSAGE_START_Y + TOLERANCE_DIS);
	coinMessageText = sf::Text(": " + std::to_string(coinCount), font, HALF_CELL_SIZE);
	coinMessageText.setFillColor(sf::Color::Black);
	coinMessageText.setPosition(viewX + COIN_MESSAGE_START_X + HALF_CELL_SIZE + TOLERANCE_DIS, COIN_MESSAGE_START_Y - TOLERANCE_DIS);

	window.draw(coinMessageSprite);
	window.draw(coinMessageText);
	/*----------------------------score Message------------------------------*/
	scoreText = sf::Text("SCORE: " + std::to_string(score), font, HALF_CELL_SIZE);
	scoreText.setFillColor(sf::Color::Black);
	scoreText.setPosition(viewX + SCORE_START_X, SCORE_START_Y);
	window.draw(scoreText);
}

sf::FloatRect Erio::getHitbox() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}