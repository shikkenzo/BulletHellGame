#pragma once
#include "SFML/Graphics.hpp"


struct Bullet
{
	sf::Vector2f position = {};
	sf::Vector2f fireDirection = {};
	sf::Vector2f addedMovement = {};
	sf::Color bulletColor = { 7, 252, 3 };
	float projectileSpeed = 1000.f;
	float projectileSize = 1.5f;

	void render(sf::RenderWindow& window,
		sf::Sprite projectileSprite
	);

	void update(float deltaTime);

	bool isEnemy = 0;
};