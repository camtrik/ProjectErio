#pragma once
#include <chrono>
#include <array>
#include <vector>

constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 320;
constexpr unsigned short SCREEN_RESIZE = 2;

constexpr unsigned short CELL_SIZE = 16;

constexpr float ERIO_SPEED = 3;
constexpr float ERIO_JUMP = 3;
constexpr float ERIO_ACCELERATE = 0.25f;
constexpr unsigned short ERIO_JUMP_TIMER = 12;

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
