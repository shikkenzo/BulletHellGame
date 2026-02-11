#include <SFML/Graphics.hpp>
#include <iostream>

#include "platformInput.h"
#include "gameLogic.h"

int mapSfmlKeyToPlatformButton(sf::Keyboard::Key key)
{
    using namespace platform;

    // Letters A-Z
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
        return Button::A + (key - sf::Keyboard::A);

    // Numbers 0-9 (NR0 - NR9)
    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
        return Button::NR0 + (key - sf::Keyboard::Num0);

    // Special keys mapping
    switch (key)
    {
    case sf::Keyboard::Space: return Button::Space;
    case sf::Keyboard::Enter: return Button::Enter;
    case sf::Keyboard::Escape: return Button::Escape;
    case sf::Keyboard::Up: return Button::Up;
    case sf::Keyboard::Down: return Button::Down;
    case sf::Keyboard::Left: return Button::Left;
    case sf::Keyboard::Right: return Button::Right;
    case sf::Keyboard::LControl: return Button::LeftCtrl;
    case sf::Keyboard::Tab: return Button::Tab;

    default: return -1; // Not mapped
    }
}

sf::View m_camera;
int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML");

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile(RESOURCES_PATH "SpaceBackground.png");
    sf::Texture bgDustTexture;
    bgDustTexture.loadFromFile(RESOURCES_PATH "SpaceDust.png");
    sf::Texture bgNebulaeTexture;
    bgNebulaeTexture.loadFromFile(RESOURCES_PATH "SpaceNebulae.png");
    sf::Texture bgStarsTexture;
    bgStarsTexture.loadFromFile(RESOURCES_PATH "SpaceStars.png");


    sf::Texture playerTexture;
    playerTexture.loadFromFile(RESOURCES_PATH "Spaceship/Main Ship/Main Ship - Bases/PNGs/Main Ship - Base - Full health.png");

    setSprites(backgroundTexture, bgDustTexture, bgNebulaeTexture, bgStarsTexture, playerTexture);
    
    window.setFramerateLimit(60);

    if (!initGameplay(window))
        return 0;

    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
			{
				// Adjust the viewport when the window is resized
				sf::View camera (sf::FloatRect (0, 0, event.size.width, event.size.height));
				m_camera = camera;
				window.setView(m_camera);
			}
            // Keyboard key pressed/released
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                bool state = (event.type == sf::Event::KeyPressed);
                int btn = mapSfmlKeyToPlatformButton(event.key.code);
                if (btn != -1)
                    platform::internal::setButtonState(btn, state);
            }

            // Text input (for typing)
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                    platform::internal::addToTypedInput(static_cast<char>(event.text.unicode));
            }

            // Mouse buttons
            if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
            {
                bool state = (event.type == sf::Event::MouseButtonPressed);
                if (event.mouseButton.button == sf::Mouse::Left)
                    platform::internal::setLeftMouseState(state);
                else if (event.mouseButton.button == sf::Mouse::Right)
                    platform::internal::setRightMouseState(state);
            }

            // Window lost focus: reset inputs
            if (event.type == sf::Event::LostFocus)
                platform::internal::resetInputsToZero();
        }

        // Update buttons (pressed/held/released/typed)
        platform::internal::updateAllButtons(deltaTime);

        // Run game logic frame
        if (!gameplayFrame(deltaTime, window))
            break;

        // Reset typed input buffer every frame
        platform::internal::resetTypedInput();

        // Render
        window.clear();

        drawGame(window);

        window.display();
    }

    closeGameLogic();
    return 0;
}