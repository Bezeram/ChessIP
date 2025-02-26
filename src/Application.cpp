#include "Application.h"

ChessAI::Application::Application()
    : m_Window(sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "ChessAI"))
    , m_Renderer(m_Window.getSize())
{
	ChessAI::ResourceManager::Initialise();
    
    m_Window.setFramerateLimit(400);
}

ChessAI::Application::~Application()
{
	ChessAI::ResourceManager::Shutdown();
}

void ChessAI::Application::Run()
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

                if (keyPressed->scancode == sf::Keyboard::Scan::K)
                {
                    if (keyPressed->shift)
                        g_AdjustableK -= 0.1f;
                    else
                        g_AdjustableK += 0.1f;
                }
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePosition = sf::Vector2f(mousePressed->position.x, mousePressed->position.y);

                    if (m_Renderer.IsMouseOnBoard(mousePosition))
                    {
                        // Convert to int
                        sf::Vector2i cellIndex = m_Renderer.MouseCellIndex(mousePosition);
                        int selectPosition = cellIndex.y * 8 + cellIndex.x;
                        std::cout << "Mouse cell select " << cellIndex.x << ' ' << cellIndex.y << std::endl;
                        m_SelectedSquare = selectPosition;
                    }
                    else
                        m_SelectedSquare = -1;
                }

            }
            else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseReleased->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePosition = sf::Vector2f(mouseReleased->position.x, mouseReleased->position.y);
                    if (m_Renderer.IsMouseOnBoard(mousePosition))
                    {
                        sf::Vector2i cellIndex = m_Renderer.MouseCellIndex(mousePosition);
                        int targetPosition = cellIndex.y * 8 + cellIndex.x;
                        std::cout << "Mouse cell target " << cellIndex.x << ' ' << cellIndex.y << std::endl;
                        // Make move if a square was previously selected
                        if (m_SelectedSquare != -1)
                        {
                            m_Board.MakeMove(Move(m_SelectedSquare, targetPosition));
                        }
                    }
                    
                    // Reset selection
                    m_SelectedSquare = -1;
                }
            }
        }

        switch (m_GameType)
        {
        case GameType::PLAYER_VS_PLAYER:
            
            break;
        }

        m_Window.clear();

        m_Window.setView(viewport);
        m_Renderer.DrawBoard(m_Window, m_Board, m_SelectedSquare);

        m_Window.display();
    }
}
