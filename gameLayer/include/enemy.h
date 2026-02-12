#include "SFML/Graphics.hpp"

struct Enemy
{
	sf::Vector2f position = {};
	sf::Vector2f viewDirection = { 0.f, 1.f };
	float health = 1.f;
	float speed = 1500.f;
	float turnSpeed = 2.f;
	float fireFrequency = 0.1f;
	float fireRange = 1.8f;
	float bulletSpeed = 1000.f;
	float fireTimer;
	void render(sf::RenderWindow& window, sf::Sprite shipSprite);
	bool update(float deltaTime, sf::Vector2f playerPos);
};
