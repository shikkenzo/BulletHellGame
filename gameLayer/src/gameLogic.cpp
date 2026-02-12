#include "gameLogic.h"
#include "platformInput.h"
#include "backgroundRenderer.h"
#include "bullet.h"
#include <iostream>
#include <vector>
#include <cmath>

struct GameplayData {
	sf::Vector2f playerPos = { 0.f , 0.f };
	sf::Vector2f playerOffset = { 0.f , 0.f };
	float playerRotation = 0.f;
	float playerSpeed = 500.f;
	float health = 1.f;

	std::vector<Bullet> projectiles;
};
GameplayData data;

float shipSize = 5.f;

float backgroundScale = 5.f;

constexpr int BACKGROUND_LAYERS = 4;

Background backgroundRenderer[BACKGROUND_LAYERS];
sf::Sprite backgroundSprite;
sf::Sprite backgroundDustSprite;
sf::Sprite backgroundNebulaeSprite;
sf::Sprite backgroundStarsSprite;
sf::Sprite projectileSprite;
sf::Sprite playerSprite;
void setSprites(sf::Texture& background, sf::Texture& bgDust, sf::Texture& bgNebulae, sf::Texture& bgStars, sf::Texture& projectile, sf::Texture& player)
{
	backgroundSprite.setTexture(background);
	backgroundSprite.setScale(backgroundScale, backgroundScale);

	backgroundDustSprite.setTexture(bgDust);
	backgroundDustSprite.setScale(backgroundScale, backgroundScale);

	backgroundNebulaeSprite.setTexture(bgNebulae);
	backgroundNebulaeSprite.setScale(backgroundScale, backgroundScale);

	backgroundStarsSprite.setTexture(bgStars);
	backgroundStarsSprite.setScale(backgroundScale, backgroundScale);

	backgroundRenderer[0].sprite = backgroundSprite;
	backgroundRenderer[1].sprite = backgroundDustSprite;
	backgroundRenderer[2].sprite = backgroundNebulaeSprite;
	backgroundRenderer[3].sprite = backgroundStarsSprite;

	backgroundRenderer[0].parallaxStrength = 0.f;
	backgroundRenderer[1].parallaxStrength = 0.5f;
	backgroundRenderer[2].parallaxStrength = 0.7f;
	backgroundRenderer[3].parallaxStrength = 1.f;

	backgroundRenderer[0].backgroundScale = backgroundScale;
	backgroundRenderer[1].backgroundScale = backgroundScale;
	backgroundRenderer[2].backgroundScale = backgroundScale;
	backgroundRenderer[3].backgroundScale = backgroundScale;

	projectileSprite.setTexture(projectile);

	playerSprite.setTexture(player);
	playerSprite.setScale(shipSize, shipSize);
	sf::FloatRect bounds = playerSprite.getLocalBounds();
	playerSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void restartGame()
{
	data = {};
}
bool initGameplay(sf::RenderWindow& window) {
	return true;
}

bool gameplayFrame(float deltaTime, sf::RenderWindow& window) {
#pragma region player movement

	sf::Vector2f l_move;
	if (platform::isButtonHeld(sf::Keyboard::W))
	{
		l_move.y = -1;
	}
	if (platform::isButtonHeld(sf::Keyboard::S))
	{
		l_move.y = 1;
	}
	if (platform::isButtonHeld(sf::Keyboard::A))
	{
		l_move.x = -1;
	}
	if (platform::isButtonHeld(sf::Keyboard::D))
	{
		l_move.x = 1;
	}

	if (l_move.x != 0 || l_move.y != 0)
	{
		l_move = l_move / (std::sqrt((l_move.x * l_move.x) + (l_move.y * l_move.y)));
	}

	/*std::cout << l_move.x;
	std::cout << l_move.y;
	std::cout << "\n";*/

	sf::View view = window.getView();
	sf::Vector2f cameraVelocity = deltaTime * data.playerSpeed * sf::Vector2f{ 0.f, -1.f };
	view.move(cameraVelocity);
	window.setView(view);

	data.playerOffset += deltaTime * data.playerSpeed * l_move;

	if (l_move.x != 0 || l_move.y != 0)
	{
		data.playerRotation = vectorToAngle(l_move);
		playerSprite.setRotation(data.playerRotation);
	}

	data.playerPos = view.getCenter() + data.playerOffset;
	playerSprite.setPosition(data.playerPos);

#pragma endregion


#pragma region projectiles
	if (platform::isButtonPressedOn(platform::Button::Space))
	{
		//std::cout << "SPACE PRESSED\n";
		Bullet b;
		b.position = data.playerPos;
		b.fireDirection = angleToVector(data.playerRotation);
		b.addedMovement = cameraVelocity;
		data.projectiles.push_back(b);
		//std::cout << data.projectiles.size();
	}

	for (int i = 0; i < data.projectiles.size(); i++)
	{
		sf::Vector2f direction = data.projectiles[i].position - view.getCenter();
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (distance > 2500.f)
		{
			data.projectiles.erase(data.projectiles.begin() + i);
			i--;
			continue;
		}

		data.projectiles[i].update(deltaTime);
	}

#pragma endregion

	return true;
}

void closeGameLogic() {

}

void drawGame(sf::RenderWindow& window)
{
	for (int i = 0; i < BACKGROUND_LAYERS; i++)
	{
		backgroundRenderer[i].render(window);
	}
	for (Bullet& b : data.projectiles)
	{
		b.render(window, projectileSprite);
	}
	window.draw(playerSprite);
}

float vectorToAngle(sf::Vector2f direction)
{
	return (std::atan2f(direction.y, direction.x) * 180.f / 3.14159265f) + 90;
}

sf::Vector2f angleToVector(float angleDegrees)
{
	float radians = (angleDegrees - 90.f) * 3.14159265f / 180.f;
	return sf::Vector2f(std::cos(radians), std::sin(radians));
}