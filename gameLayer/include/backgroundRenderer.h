#pragma once 
#include "SFML/graphics.hpp"

struct Background
{
	float backgroundScale = 1.f;
	sf::Sprite sprite;

	float parallaxStrength = 1;

	void Background::render(sf::RenderWindow& window);
};
