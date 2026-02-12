#include <bullet.h>

void Bullet::render(sf::RenderWindow& window,
	sf::Texture projectileTextures, sf::IntRect textureCoords)
{
	float rotation = atan2(fireDirection.y, -fireDirection.x);
	rotation = rotation * 180.f / 3.14159265f + 90.f;

	if (isEnemy)
	{

	}

	for (int i = 0; i < 5; i++)
	{
		sf::Color color(1 * (i + 4) / 5.f, 1 * (i + 4) / 5.f, 1 * (i + 4) / 5.f, (i + 1) / 5.f);
		sf::Sprite projectileSprite;
		projectileSprite.setTexture(projectileTextures);
		projectileSprite.setTextureRect(textureCoords);
		window.draw(projectileSprite);
	}
}

void Bullet::update(float deltaTime)
{
	position += fireDirection * deltaTime * projectileSpeed + addedMovement;
}
