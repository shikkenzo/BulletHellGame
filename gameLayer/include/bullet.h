#pragma once
#include "SFML/Graphics.hpp"


struct Bullet
{
	sf::Vector2f position = {};
	sf::Vector2f fireDirection = {};
	sf::Vector2f addedMovement = {};
	float projectileSpeed = 4000.f;
	float projectileSize = 100.f;

	void render(sf::RenderWindow& window,
		sf::Texture projectileTextures,
		sf::IntRect textureCoords
	);

	void update(float deltaTime);

	bool isEnemy = 0;
};