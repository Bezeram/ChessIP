#include "Application.h"

ChessAI::Application::Application()
    : m_Window(sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "ChessAI"))
{
	ChessAI::ResourceManager::Initialise();
    
    m_Window.setFramerateLimit(400);
}

ChessAI::Application::~Application()
{
	ChessAI::ResourceManager::Shutdown();
}

void ChessAI::Application::run()
{
    sf::View viewport(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(m_Window.getSize().x, m_Window.getSize().y)));

    bool isRunning = true;
    while (isRunning)
    {
        while (const std::optional event = m_Window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                m_Window.close();
                isRunning = false;
                return;
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
                    m_Window.close();
                    isRunning = false;
                    return;
                }
            }
        }

        m_Window.clear();

        m_Window.setView(viewport);
        m_Renderer.DrawBoard(m_Window, m_Board);

        m_Window.display();
    }
}
