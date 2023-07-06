#pragma once
#include <chrono>
#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 320;
constexpr unsigned short SCREEN_RESIZE = 2;

constexpr unsigned short CELL_SIZE = 16;
constexpr unsigned short HALF_CELL_SIZE = 8;
constexpr unsigned short UPDATE_AREA = CELL_SIZE * 8;

constexpr unsigned short QUESTION_ANIMATION_SPEED = 4;
constexpr unsigned short COIN_ANIMATION_SPEED = 4;
constexpr float COIN_JUMP_SPEED = 5;

//ERIO
constexpr float ERIO_SPEED = 2.5;
constexpr float ERIO_JUMP = 4;
constexpr float ERIO_ACCELERATE = 0.25f;
constexpr unsigned short ERIO_JUMP_TIMER = 10;
constexpr unsigned short ERIO_ANIMATION_SPEED = 4;
constexpr unsigned short ERIO_DEATH_DURATION = 32;

constexpr float BOMB_SPEED = 0.8f;

constexpr float SLIME_SPEED = 0.8f;
constexpr short SLIME_DEATH_DURATION = 32;

constexpr float GRAVITY = 0.2f;

// Message
constexpr unsigned short BOMB_MESSAGE_START_X = HALF_CELL_SIZE * 10;
constexpr unsigned short BOMB_MESSAGE_START_Y = HALF_CELL_SIZE;
constexpr unsigned short COIN_MESSAGE_START_X = HALF_CELL_SIZE * 2;
constexpr unsigned short COIN_MESSAGE_START_Y = HALF_CELL_SIZE;
constexpr unsigned short SCORE_START_X = SCREEN_WIDTH - HALF_CELL_SIZE * 10;
constexpr unsigned short SCORE_START_Y = HALF_CELL_SIZE;
constexpr unsigned short TOLERANCE_DIS = 1;

// Score
constexpr unsigned short COIN_SCORE = 100;
constexpr unsigned short SLIME_SCORE = 250;

constexpr unsigned short FPS = 60;
constexpr std::chrono::microseconds FRAME_DURATION(1000000 / FPS);


enum Cell
{
	ActivatedQuestionBlock,
	Brick,
	Coin,
	Empty,
	Pipe,
	QuestionBlock,
	Wall
};

typedef std::vector<std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;

void loadAudioFile(const std::string path, sf::SoundBuffer& buffer, sf::Sound& sound);
void loadMusicFile(const std::string path, sf::Music& music);

struct SimpleObject
{
	float x;
	float y;
	float verticalSpeed;
	float horizontalSpeed;

	SimpleObject(float p_x, float p_y, float p_horizontalSpeed, float p_verticalSpeed) :
		x(p_x),
		y(p_y),
		verticalSpeed(p_verticalSpeed),
		horizontalSpeed(p_horizontalSpeed)
	{
	}
};