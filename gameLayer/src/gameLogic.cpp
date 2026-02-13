#include "gameLogic.h"
#include "platformInput.h"
#include "backgroundRenderer.h"
#include "bullet.h"
#include "enemy.h"
#include "vectorMath.h"
#include <iostream>
#include <vector>
#include <cmath>

struct GameplayData {
	sf::Vector2f playerPos = { 0.f , 0.f };
	sf::Vector2f playerOffset = { 0.f , 0.f };
	sf::Color playerColor = { 125, 125, 252 };
	sf::Sprite playerSprite;
	float playerRotation = 0.f;
	float playerSpeed = 700.f;
	float scrollSpeed = 500.f;
	float health = 1.f;
	int level = 1;

	std::vector<Bullet> projectiles;
	std::vector<Enemy> enemies;
};
GameplayData m_data;

float m_shipScale = 5.f;
float m_collisionSize;
float m_backgroundScale = 5.f;

constexpr int BACKGROUND_LAYERS = 4;
constexpr bool ROTATE_WITH_MOUSE = true;

Background m_backgroundRenderer[BACKGROUND_LAYERS];
sf::Sprite m_backgroundSprite;
sf::Sprite m_backgroundDustSprite;
sf::Sprite m_backgroundNebulaeSprite;
sf::Sprite m_backgroundStarsSprite;
sf::Sprite m_projectileSprite;
sf::Sprite m_playerSprite0;
sf::Sprite m_playerSprite1;
sf::Sprite m_playerSprite2;
sf::Sprite m_playerSprite3;
sf::Sprite m_enemySprite0;
sf::Sprite m_enemySprite1;
sf::Sprite m_enemySprite2;
sf::Sprite m_enemySprite3;
void setSprites(sf::Texture& background, sf::Texture& bgDust, sf::Texture& bgNebulae, sf::Texture& bgStars, sf::Texture& projectile, sf::Texture& player0, sf::Texture& player1, sf::Texture& player2, sf::Texture& player3, sf::Texture& enemy0, sf::Texture& enemy1, sf::Texture& enemy2, sf::Texture& enemy3)
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

	m_playerSprite0.setTexture(player0);
	m_playerSprite1.setTexture(player1);
	m_playerSprite2.setTexture(player2);
	m_playerSprite3.setTexture(player3);

	m_playerSprite0.setScale(m_shipScale, m_shipScale);
	m_playerSprite1.setScale(m_shipScale, m_shipScale);
	m_playerSprite2.setScale(m_shipScale, m_shipScale);
	m_playerSprite3.setScale(m_shipScale, m_shipScale);

	sf::FloatRect l_playerBounds = m_playerSprite0.getLocalBounds();
	m_playerSprite0.setOrigin(l_playerBounds.width / 2.f, l_playerBounds.height / 2.f);
	m_playerSprite1.setOrigin(l_playerBounds.width / 2.f, l_playerBounds.height / 2.f);
	m_playerSprite2.setOrigin(l_playerBounds.width / 2.f, l_playerBounds.height / 2.f);
	m_playerSprite3.setOrigin(l_playerBounds.width / 2.f, l_playerBounds.height / 2.f);

	m_enemySprite0.setTexture(enemy0);
	m_enemySprite1.setTexture(enemy1);
	m_enemySprite2.setTexture(enemy2);
	m_enemySprite3.setTexture(enemy3);

	m_enemySprite0.setScale(m_shipScale, m_shipScale);
	m_enemySprite1.setScale(m_shipScale, m_shipScale);
	m_enemySprite2.setScale(m_shipScale, m_shipScale);
	m_enemySprite3.setScale(m_shipScale, m_shipScale);

	sf::FloatRect l_enemyBounds = m_enemySprite0.getLocalBounds();
	m_enemySprite0.setOrigin(l_enemyBounds.width / 2.f, l_enemyBounds.height / 2.f);
	m_enemySprite1.setOrigin(l_enemyBounds.width / 2.f, l_enemyBounds.height / 2.f);
	m_enemySprite2.setOrigin(l_enemyBounds.width / 2.f, l_enemyBounds.height / 2.f);
	m_enemySprite3.setOrigin(l_enemyBounds.width / 2.f, l_enemyBounds.height / 2.f);

	m_data.playerSprite = m_playerSprite0;
}

void restartGame()
{
	m_data = {};
	m_collisionSize = m_playerSprite0.getLocalBounds().getSize().x;
	spawnEnemy();
}
bool initGameplay(sf::RenderWindow& window) {
	restartGame();
	return true;
}

bool gameplayFrame(float deltaTime, sf::RenderWindow& window) {

	if (platform::isButtonPressedOn(platform::Button::R))
	{
		restartGame();
	}

	sf::View l_view = window.getView();
	sf::Vector2f l_screenCenter = l_view.getCenter();

	sf::Vector2f l_mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	sf::Vector2f l_mouseDir = l_mouseWorldPos - m_data.playerPos;

	sf::Vector2f l_cameraVelocity = deltaTime * m_data.scrollSpeed * sf::Vector2f{ 0.f, -1.f };


#pragma region player shoot
	if (platform::isLMousePressed())
	{
		Bullet b;
		b.position = m_data.playerPos;
		b.fireDirection = angleToVector(m_data.playerRotation);
		b.addedMovement = l_cameraVelocity;
		m_data.projectiles.push_back(b);
		//std::cout << data.projectiles.size();
	}

	/*for (int i = 0; i < m_data.projectiles.size(); i++)
	{
		sf::Vector2f l_direction = m_data.projectiles[i].position - l_screenCenter;
		float l_distance = getVectorMagnitude(l_direction);
		if (l_distance > 2500.f)
		{
			m_data.projectiles.erase(m_data.projectiles.begin() + i);
			i--;
			continue;
		}

		m_data.projectiles[i].update(deltaTime);
	}*/

#pragma endregion


#pragma region projectile logic

	for (int i = 0; i < m_data.projectiles.size(); i++)
	{
		bool breakBothLoops = false;

		if (getVectorMagnitude(m_data.projectiles[i].position - m_data.playerPos) > 5'000)
		{
			m_data.projectiles.erase(m_data.projectiles.begin() + i);
			i--;
			continue;
		}

		if (!m_data.projectiles[i].isEnemy)
		{
			for (int e = 0; e < m_data.enemies.size(); e++)
			{
				if (intersectBullet(m_data.projectiles[i].position, m_data.enemies[e].position, m_collisionSize))
				{
					m_data.enemies[e].health -= 0.1f;
					//std::cout << "hit";
					//std::cout << "\n";

					if (m_data.enemies[e].health <= 0)
					{
						m_data.enemies.erase(m_data.enemies.begin() + e);
					}
					else if (m_data.enemies[e].health <= 0.25f)
					{
						m_data.enemies[e].enemySprite = m_enemySprite3;
					}
					else if (m_data.enemies[e].health <= 0.5f)
					{
						m_data.enemies[e].enemySprite = m_enemySprite2;
					}
					else if (m_data.enemies[e].health <= 0.75)
					{
						m_data.enemies[e].enemySprite = m_enemySprite1;
					}
					else
					{
						m_data.enemies[e].enemySprite = m_enemySprite0;
					}

					m_data.projectiles.erase(m_data.projectiles.begin() + i);
					i--;
					breakBothLoops = true;
					continue;
				}
			}

			if (breakBothLoops)
			{
				continue;
			}
		}
		else
		{
			if (intersectBullet(m_data.projectiles[i].position, m_data.playerPos, m_collisionSize))
			{
				m_data.health -= 0.1f;
				m_data.projectiles.erase(m_data.projectiles.begin() + i);
				i--;
				continue;
			}
		}

		m_data.projectiles[i].update(deltaTime);
	}

	if (m_data.health <= 0)
	{
		restartGame();
	}
	else
	{
		m_data.health += deltaTime * 0.01f;
		m_data.health = std::clamp(m_data.health, 0.f, 1.f);

		if (m_data.health <= 0.25f)
		{
			m_data.playerSprite = m_playerSprite3;
		}
		else if (m_data.health <= 0.5f)
		{
			m_data.playerSprite = m_playerSprite2;
		}
		else if (m_data.health <= 0.75)
		{
			m_data.playerSprite = m_playerSprite1;
		}
		else
		{
			m_data.playerSprite = m_playerSprite0;
		}
	}

#pragma endregion 


#pragma region player movement

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
		l_move = normalizeVector(l_move);
	}

	/*std::cout << l_move.x;
	std::cout << l_move.y;
	std::cout << "\n";*/

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
	m_data.playerSprite.setRotation(m_data.playerRotation);

	m_data.playerPos = l_screenCenter + m_data.playerOffset;
	m_data.playerSprite.setPosition(m_data.playerPos);

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
	m_data.playerOffset = m_data.playerPos - l_screenCenter;


#pragma endregion


#pragma region enemies

	/*if (platform::isButtonPressedOn(platform::Button::Space))
	{
		spawnEnemy();
	}*/

	if (m_data.enemies.size() == 0)
	{
		for (int i = 0; i < m_data.level; i++)
		{
			spawnEnemy();
		}
		m_data.level++;
	}

	for (int i = 0; i < m_data.enemies.size(); i++)
	{
		sf::Vector2f l_direction = m_data.enemies[i].position - l_screenCenter;
		float l_distance = getVectorMagnitude(l_direction);
		if (l_distance > 2500.f)
		{
			m_data.enemies.erase(m_data.enemies.begin() + i);
			i--;
			continue;
		}

		if (m_data.enemies[i].update(deltaTime, m_data.playerPos, l_cameraVelocity))
		{
			Bullet b;
			b.position = m_data.enemies[i].position;
			b.fireDirection = m_data.enemies[i].viewDirection;
			b.addedMovement = l_cameraVelocity;
			b.isEnemy = true;
			//b.bulletColor = m_data.enemies[i].enemyColor;
			m_data.projectiles.push_back(b);
		}
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
		m_backgroundRenderer[i].render(window);
	}


	for (Bullet& b : m_data.projectiles)
	{
		b.render(window, m_projectileSprite);
	}


	for (Enemy& e : m_data.enemies)
	{
		e.render(window);
	}

	m_data.playerSprite.setColor(m_data.playerColor); 
	window.draw(m_data.playerSprite);
}

void spawnEnemy()
{
	Enemy enemy;
	enemy.position = m_data.playerPos;

	float l_offsetValue = 2000.f;
	sf::Vector2f l_offset = angleToVector(rand() % 360) * l_offsetValue;
	enemy.position += l_offset;

	enemy.speed = 700 * rand() % 1000;
	enemy.turnSpeed = 2.f + (rand() % 1000) / 500.f;
	enemy.fireRange = 1.5 + (rand() % 1000) / 2000.f;
	enemy.fireFrequency = 0.1 + (rand() % 1000) / 500.f;
	enemy.bulletSpeed = rand() % 3000 + 1000;
	enemy.enemySprite = m_enemySprite0;

	m_data.enemies.push_back(enemy);
}

bool intersectBullet(sf::Vector2f bulletPos, sf::Vector2f shipPos, float shipSize)
{
	return getVectorMagnitude(bulletPos - shipPos) <= shipSize;
}
