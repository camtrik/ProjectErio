#pragma once
#include <chrono>
#include <array>
#include <vector>

constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 320;
constexpr unsigned short SCREEN_RESIZE = 2;

constexpr unsigned short CELL_SIZE = 16;
constexpr unsigned short UPDATE_AREA = CELL_SIZE * 8;

constexpr float ERIO_SPEED = 2.5;
constexpr float ERIO_JUMP = 4;
constexpr float ERIO_ACCELERATE = 0.25f;
constexpr unsigned short ERIO_JUMP_TIMER = 10;
constexpr unsigned short ERIO_ANIMATION_SPEED = 4;
constexpr unsigned short ERIO_DEATH_DURATION = 32;

constexpr float SLIME_SPEED = 0.8f;
constexpr short SLIME_DEATH_DURATION = 32;

constexpr float GRAVITY = 0.2f;


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
