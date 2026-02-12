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
	float playerSpeed = 700.f;
	float scrollSpeed = 500.f;
	float health = 1.f;

	std::vector<Bullet> projectiles;
};
GameplayData m_data;

float m_shipSize = 5.f;
float m_backgroundScale = 5.f;

constexpr int BACKGROUND_LAYERS = 4;
constexpr bool ROTATE_WITH_MOUSE = true;

Background m_backgroundRenderer[BACKGROUND_LAYERS];
sf::Sprite m_backgroundSprite;
sf::Sprite m_backgroundDustSprite;
sf::Sprite m_backgroundNebulaeSprite;
sf::Sprite m_backgroundStarsSprite;
sf::Sprite m_projectileSprite;
sf::Sprite m_playerSprite;
void setSprites(sf::Texture& background, sf::Texture& bgDust, sf::Texture& bgNebulae, sf::Texture& bgStars, sf::Texture& projectile, sf::Texture& player)
{
	m_backgroundSprite.setTexture(background);
	m_backgroundSprite.setScale(m_backgroundScale, m_backgroundScale);

	m_backgroundDustSprite.setTexture(bgDust);
	m_backgroundDustSprite.setScale(m_backgroundScale, m_backgroundScale);

	m_backgroundNebulaeSprite.setTexture(bgNebulae);
	m_backgroundNebulaeSprite.setScale(m_backgroundScale, m_backgroundScale);

	m_backgroundStarsSprite.setTexture(bgStars);
	m_backgroundStarsSprite.setScale(m_backgroundScale, m_backgroundScale);

	m_backgroundRenderer[0].sprite = m_backgroundSprite;
	m_backgroundRenderer[1].sprite = m_backgroundDustSprite;
	m_backgroundRenderer[2].sprite = m_backgroundNebulaeSprite;
	m_backgroundRenderer[3].sprite = m_backgroundStarsSprite;

	m_backgroundRenderer[0].parallaxStrength = 0.f;
	m_backgroundRenderer[1].parallaxStrength = 0.5f;
	m_backgroundRenderer[2].parallaxStrength = 0.7f;
	m_backgroundRenderer[3].parallaxStrength = 1.f;

	m_backgroundRenderer[0].backgroundScale = m_backgroundScale;
	m_backgroundRenderer[1].backgroundScale = m_backgroundScale;
	m_backgroundRenderer[2].backgroundScale = m_backgroundScale;
	m_backgroundRenderer[3].backgroundScale = m_backgroundScale;

	m_projectileSprite.setTexture(projectile);

	m_playerSprite.setTexture(player);
	m_playerSprite.setScale(m_shipSize, m_shipSize);
	sf::FloatRect bounds = m_playerSprite.getLocalBounds();
	m_playerSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void restartGame()
{
	m_data = {};
}
bool initGameplay(sf::RenderWindow& window) {
	return true;
}

bool gameplayFrame(float deltaTime, sf::RenderWindow& window) {
	sf::View l_view = window.getView();

#pragma region player movement

	sf::Vector2f l_mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	sf::Vector2f l_mouseDir = l_mouseWorldPos - m_data.playerPos;
	//std::cout << l_view.getCenter().x << "," << l_view.getCenter().y << "\n";

	sf::Vector2f l_move;
	if (platform::isButtonHeld(platform::Button::W))
	{
		l_move.y = -1;
	}
	if (platform::isButtonHeld(platform::Button::S))
	{
		l_move.y = 1;
	}
	if (platform::isButtonHeld(platform::Button::A))
	{
		l_move.x = -1;
	}
	if (platform::isButtonHeld(platform::Button::D))
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

	sf::Vector2f l_cameraVelocity = deltaTime * m_data.scrollSpeed * sf::Vector2f{ 0.f, -1.f };
	l_view.move(l_cameraVelocity);
	window.setView(l_view);

	m_data.playerOffset += deltaTime * m_data.playerSpeed * l_move;

	if (ROTATE_WITH_MOUSE)
	{
		if (l_mouseDir.x != 0 || l_mouseDir.y != 0)
		{
			m_data.playerRotation = vectorToAngle(l_mouseDir);
		}
	}
	else
	{
		if (l_move.x != 0 || l_move.y != 0)
		{
			m_data.playerRotation = vectorToAngle(l_move);
		}
	}
	m_playerSprite.setRotation(m_data.playerRotation);

	m_data.playerPos = l_view.getCenter() + m_data.playerOffset;
	m_playerSprite.setPosition(m_data.playerPos);

#pragma endregion


#pragma region projectiles
	if (platform::isLMousePressed())
	{
		Bullet b;
		b.position = m_data.playerPos;
		b.fireDirection = angleToVector(m_data.playerRotation);
		b.addedMovement = l_cameraVelocity;
		m_data.projectiles.push_back(b);
		//std::cout << data.projectiles.size();
	}

	for (int i = 0; i < m_data.projectiles.size(); i++)
	{
		sf::Vector2f l_direction = m_data.projectiles[i].position - l_view.getCenter();
		float l_distance = std::sqrt(l_direction.x * l_direction.x + l_direction.y * l_direction.y);
		if (l_distance > 2500.f)
		{
			m_data.projectiles.erase(m_data.projectiles.begin() + i);
			i--;
			continue;
		}

		m_data.projectiles[i].update(deltaTime);
	}

#pragma endregion


#pragma region window collision

	sf::Vector2i l_playerWindowPos = window.mapCoordsToPixel(m_data.playerPos);
	/*int playerHalfSizeX = m_playerSprite.getGlobalBounds().width / 2.f;
	int playerHalfSizeY = m_playerSprite.getGlobalBounds().height / 2.f;*/

	if (l_playerWindowPos.x < 0)
	{
		l_playerWindowPos.x = 0;
	}
	else if (l_playerWindowPos.x > window.getSize().x)
	{
		l_playerWindowPos.x = window.getSize().x;
	}

	if (l_playerWindowPos.y < 0)
	{
		l_playerWindowPos.y = 0;
	}
	else if (l_playerWindowPos.y > window.getSize().y)
	{
		l_playerWindowPos.y = window.getSize().y;
	}

	m_data.playerPos = window.mapPixelToCoords(l_playerWindowPos);
	m_data.playerOffset = m_data.playerPos - l_view.getCenter();
	

#pragma endregion

	return true;
}

void closeGameLogic() {

}

void drawGame(sf::RenderWindow& window)
{
	for (int i = 0; i < BACKGROUND_LAYERS; i++)
	{
		m_backgroundRenderer[i].render(window);
	}
	for (Bullet& b : m_data.projectiles)
	{
		b.render(window, m_projectileSprite);
	}
	window.draw(m_playerSprite);
}

float vectorToAngle(sf::Vector2f direction)
{
	return (std::atan2f(direction.y, direction.x) * 180.f / 3.14159265f) + 90;
}

sf::Vector2f angleToVector(float angleDegrees)
{
	float l_radians = (angleDegrees - 90.f) * 3.14159265f / 180.f;
	return sf::Vector2f(std::cos(l_radians), std::sin(l_radians));
}