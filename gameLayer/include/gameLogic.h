#pragma once
#include "SFML/Graphics.hpp"

void setSprites(sf::Texture& background, sf::Texture& bgDust, sf::Texture& bgNebulae, sf::Texture& bgStars, sf::Texture& projectile, sf::Texture& player0, sf::Texture& player1, sf::Texture& player2, sf::Texture& player3, sf::Texture& enemy0, sf::Texture& enemy1, sf::Texture& enemy2, sf::Texture& enemy3);
bool initGameplay(sf::RenderWindow& window);

bool gameplayFrame(float deltaTime, sf::RenderWindow& window);

void closeGameLogic();

void drawGame(sf::RenderWindow& window);

void spawnEnemy();

bool intersectBullet(sf::Vector2f bulletPos, sf::Vector2f shipPos, float shipSize);

