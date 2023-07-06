#pragma once
#include <SFML/Graphics.hpp>
#include <Animation.h>

class Explosion
{
public:
	float x;
	float y;
	Animation animation;

	Explosion(float p_x, float p_y) :
		x(p_x),
		y(p_y),
		animation("Resources/Images/Explosion.png", EXPLOSION_WIDTH, EXPLOSION_ANIMATION_SPEED)
	{
	}
};