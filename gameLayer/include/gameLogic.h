#pragma once
#include "SFML/Graphics.hpp"

bool initGameplay();

bool gameplayFrame(float deltaTime);

void closeGameLogic();

void drawGame(sf::RenderWindow& window);
