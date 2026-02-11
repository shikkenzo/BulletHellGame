#pragma once
#include "SFML/Graphics.hpp"

void setSprites(sf::Texture& background, sf::Texture& bgDust, sf::Texture& bgNebulae, sf::Texture& bgStars, sf::Texture& player);
bool initGameplay(sf::RenderWindow& window);

bool gameplayFrame(float deltaTime, sf::RenderWindow& window);

void closeGameLogic();

void drawGame(sf::RenderWindow& window);
