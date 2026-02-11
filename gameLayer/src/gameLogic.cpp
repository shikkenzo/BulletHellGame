#include "gameLogic.h"
#include "platformInput.h"
#include <iostream>
#include <vector>

struct GameplayData {
	float health = 1.f;
};
GameplayData data;

sf::Sprite backgroundSprite;
sf::Sprite playerSprite;
void setSprites(sf::Texture& background, sf::Texture& player)
{
	backgroundSprite.setTexture(background);
	playerSprite.setTexture(player);
}

void restartGame()
{
	data = {};
}
bool initGameplay() {
	
	return true;
}

bool gameplayFrame(float deltaTime) {
	if (platform::isButtonPressedOn(platform::Button::Space))
	{
		std::cout << "SPACE PRESSED\n";
	}
	return true;
}

void closeGameLogic() {

}

void drawGame(sf::RenderWindow& window)
{
	window.draw(backgroundSprite); 
	window.draw(playerSprite); 
}
