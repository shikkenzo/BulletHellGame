#include <backgroundRenderer.h>


void Background::render(sf::RenderWindow& window)
{
	sf::Vector2f cameraPos = window.getView().getCenter() - (sf::Vector2f)window.getViewport(window.getView()).getSize() / 2.f;

	float tileWidth = backgroundScale * sprite.getTextureRect().width;
	float tileHeight = backgroundScale * sprite.getTextureRect().height;

	sf::Vector2f parallaxDistance = { (float)cameraPos.x, (float)cameraPos.y};
	parallaxDistance *= -parallaxStrength;

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			int posX = x + int((cameraPos.x - parallaxDistance.x) / tileWidth); //view always on center background
			int posY = y + int((cameraPos.y - parallaxDistance.y) / tileHeight); //view always on center background
			
			sprite.setPosition(posX * tileWidth + parallaxDistance.x, posY * tileHeight + parallaxDistance.y);
			window.draw(sprite);
		}
	}

}
