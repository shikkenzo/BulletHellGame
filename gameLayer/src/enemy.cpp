#include "enemy.h"
#include "vectorMath.h"

void Enemy::render(sf::RenderWindow& window, sf::Sprite shipSprite)
{
	shipSprite.setPosition(position);
	window.draw(shipSprite);
}

bool Enemy::update(float deltaTime, sf::Vector2f playerPosition)
{
	sf::Vector2f directionToPlayer = playerPosition - position;
	if (getVectorMagnitude(directionToPlayer) == 0) { directionToPlayer = { 1, 0 }; }
	else { directionToPlayer = normalizeVector(directionToPlayer); }

	sf::Vector2f newDirection = {};

	bool shoot = (getVectorMagnitude(directionToPlayer + viewDirection) >= fireRange); //if player is in front of the enemy
	if (shoot)
	{
		if (fireTimer <= 0.f)
		{
			fireTimer = fireFrequency;
		}
		else
		{
			shoot = 0;
		}
		fireTimer -= deltaTime;
		if (fireTimer < 0) fireTimer = 0.f;
		//std::cout << fireTimer;
		//std::cout << "\n";
	}

	if (getVectorMagnitude(directionToPlayer + viewDirection) <= 0.2f)
	{
		if (rand() % 2)
		{
			newDirection = sf::Vector2f(directionToPlayer.y, -directionToPlayer.x); //if directionToPlayer and newDirection are opposite -> rotates the enemy 90º
		}
		else
		{
			newDirection = sf::Vector2f(-directionToPlayer.y, directionToPlayer.x); //if directionToPlayer and newDirection are opposite -> rotates the enemy 90º
		}
	}
	else
	{
		newDirection = deltaTime * turnSpeed * directionToPlayer + viewDirection;
	}
	float length = getVectorMagnitude(newDirection);
	viewDirection = normalizeVector(newDirection);

	length = std::clamp(length, 0.1f, 3.f);

	position += viewDirection * deltaTime * speed * length;

	return shoot;
}