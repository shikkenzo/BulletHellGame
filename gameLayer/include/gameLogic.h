#pragma once
#include "SFML/Graphics.hpp"

void setSprites(sf::Texture& background, sf::Texture& player);
bool initGameplay();

bool gameplayFrame(float deltaTime);

void closeGameLogic();

void drawGame(sf::RenderWindow& window);
