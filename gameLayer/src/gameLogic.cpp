#include "gameLogic.h"
#include "platformInput.h"
#include <iostream>

struct GameplayData {
	float health = 1.f;
};
GameplayData data;

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
