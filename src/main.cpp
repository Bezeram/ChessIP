#include <SFML/Graphics.hpp>

#include "Board.h"
#include "Renderer.h"

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "ChessAI");
    window.setFramerateLimit(400);

    ChessAI::Renderer renderer;
    ChessAI::Board board;

    sf::View viewport(sf::FloatRect(sf::Vector2f(0, 0 ), sf::Vector2f(window.getSize().x, window.getSize().y)));
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return 0;
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                sf::Vector2f newSize = sf::Vector2f(resized->size.x, resized->size.y);
                viewport.setCenter(newSize / 2.f);
                viewport.setSize(newSize);
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                {
                    window.close();
                    return 0;
                }
            }
        }

        
        window.clear();

        window.setView(viewport);
        renderer.DrawBoard(window, board);

        window.display();
    }
}
