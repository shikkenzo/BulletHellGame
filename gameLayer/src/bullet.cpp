#include <bullet.h>

void Bullet::render(sf::RenderWindow& window,
	sf::Sprite projectileSprite)
{
	float rotation = atan2(fireDirection.y, -fireDirection.x);
	rotation = rotation * 180.f / 3.14159265f + 90.f;

	if (isEnemy)
	{

	}

	for (int i = 0; i < 5; i++)
	{
		sf::Color color(1 * (i + 4) / 5.f, 1 * (i + 4) / 5.f, 1 * (i + 4) / 5.f, (i + 1) / 5.f);
		sf::IntRect textureRect = { 0, 0, 32, 32 };
		projectileSprite.setTextureRect(textureRect);
		projectileSprite.setOrigin(textureRect.width / 2.f, textureRect.height / 2.f);
		projectileSprite.setScale(projectileSize, projectileSize);
		projectileSprite.setPosition(position);
 		window.draw(projectileSprite);
	}
}

void Bullet::update(float deltaTime)
{
	position += fireDirection * deltaTime * projectileSpeed + addedMovement;
}
