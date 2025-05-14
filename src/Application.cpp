#include "Application.h"

Application::Application()
    : m_Window(sf::RenderWindow(sf::VideoMode({ 1280, 720 }), "Chess9"))
    , m_Viewport(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(m_Window.getSize().x, m_Window.getSize().y)))
    , m_Board(GameType::OneVOne)
    , m_Renderer(m_Window.getSize(), m_Board.GetSize())
{
	ResourceManager::Initialise();
    
    m_Window.setFramerateLimit(400);
	m_Window.setVerticalSyncEnabled(true);
}

Application::~Application()
{
	ResourceManager::Shutdown();
}

void Application::Run()
{
    while (m_IsRunning)
    {
        EventHandler();

        m_Window.clear();

        m_Window.setView(m_Viewport);
        m_Renderer.DrawBoard(m_Window, m_Board, m_SelectedSquare, m_PreviousMove);

        m_Window.display();
    }
}

void Application::EventHandler()
{
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_Window.close();
            m_IsRunning = false;
            return;
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            sf::Vector2f newSize = sf::Vector2f(resized->size.x, resized->size.y);
            m_Viewport.setCenter(newSize / 2.f);
            m_Viewport.setSize(newSize);
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
            {
                m_Window.close();
                m_IsRunning = false;
                return;
            }

            if (keyPressed->scancode == sf::Keyboard::Scan::K)
            {
                if (keyPressed->shift)
                    Global::AdjustableK -= 0.1f;
                else
                    Global::AdjustableK += 0.1f;
            }
        }
        else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            // Initial piece is selected
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePosition = sf::Vector2f(mousePressed->position.x, mousePressed->position.y);

                sf::Vector2i cellIndex = m_Renderer.MouseCellIndex(mousePosition);
                if (m_Renderer.IsMouseOnBoard(mousePosition) && m_Board.IsValidPieceByTurn(cellIndex))
                    m_SelectedSquare = cellIndex;
                else
                    m_SelectedSquare = GlobalConstants::NullPosition;
            }

        }
        else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouseReleased->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePosition = sf::Vector2f(mouseReleased->position.x, mouseReleased->position.y);
                if (m_Renderer.IsMouseOnBoard(mousePosition))
                {
                    sf::Vector2i mouseCellIndex = m_Renderer.MouseCellIndex(mousePosition);
                    // Make move if a square was previously selected
                    if (m_SelectedSquare != GlobalConstants::NullPosition)
                    {
                        PieceMove move = PieceMove(m_SelectedSquare, mouseCellIndex);
                        bool validMove = m_Board.MakeMove(move.StartSquare, move);
                        if (validMove)
                        {
                            m_PreviousMove = move;
                        }
                    }
                }

                // Reset selection
                m_SelectedSquare = GlobalConstants::NullPosition;
            }
        }
    }
}
