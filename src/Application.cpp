#include "Application.h"

Application::Application(WindowSettings settings)
    : m_Window(sf::RenderWindow(
        (settings.Resolution.x == 0) ? sf::VideoMode::getDesktopMode() : sf::VideoMode(settings.Resolution),
        "Chess9", sf::Style::Default, settings.State)
    )
    , m_Viewport(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(m_Window.getSize().x, m_Window.getSize().y)))
    , m_Board(std::make_shared<Board>(GameType::OneVOne))
    , m_Renderer(m_Window.getSize(), m_Board->GetSize())
{
	ResourceManager::Initialise();
	SoundPlayer::Initialize();
    
    m_Window.setFramerateLimit(400);
	m_Window.setVerticalSyncEnabled(true);

	m_Board->Init1v1Game(m_Board);
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
        m_Renderer.DrawBoard(m_Window, *m_Board, m_SelectedSquare, m_MoveType, m_PreviousMove);
		m_Renderer.DrawHUD(m_Window.getSize(), m_Board->GetSize());

        m_Window.display();

        SoundPlayer::GetInstance().Update();
    }
}

void Application::EventHandler()
{
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            // Window close button
            m_Window.close();
            m_IsRunning = false;
            return;
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            // Reset viewport to match new window size
            sf::Vector2f newSize = sf::Vector2f(resized->size.x, resized->size.y);
            m_Viewport.setCenter(newSize / 2.f);
            m_Viewport.setSize(newSize);
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // Quit
            if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
            {
                m_Window.close();
                m_IsRunning = false;
                return;
            }

            // Global variable adjusted by key
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
            Global::MouseLeftPressed = true;

            MoveHandler_MousePressed(mousePressed);
        }
        else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            Global::MouseLeftPressed = false;

            MoveHandler_MouseReleased(mouseReleased);
        }
    }
}

void Application::MoveHandler_MousePressed(const sf::Event::MouseButtonPressed* mousePressed)
{
    // Assign move type. 
    // Left click generates legal piece moves
    // Right click generates legal action moves
    MoveType moveTypePressed = MoveType::None;
    if (mousePressed->button == sf::Mouse::Button::Left)
        moveTypePressed = MoveType::Move;
    if (mousePressed->button == sf::Mouse::Button::Right)
        moveTypePressed = MoveType::Action;

    // Initial piece is selected
    sf::Vector2f mousePosition = sf::Vector2f(mousePressed->position.x, mousePressed->position.y);
    sf::Vector2i targetSquare = m_Renderer.MouseCellIndex(m_Window.getSize().y, mousePosition);

    if (m_MoveType != MoveType::None && m_MoveType == moveTypePressed &&
        m_SelectedSquare != GlobalConstants::NullPosition)
    {
        // If the square is already selected make move using new selection
        if (m_SelectedSquare != targetSquare)
        {
            if (IsCellInBounds(targetSquare, m_Board->GetSize()));
            {
                // Make move if a square was previously selected
                PieceMove move = PieceMove(m_SelectedSquare, targetSquare);
                const auto& selectedPiece = (*m_Board)[move.StartSquare];

                if (selectedPiece.get() != nullptr)
                {
                    ActionMove actionMove = selectedPiece->IsLegalMove(move, m_MoveType);
                    if (actionMove != GlobalConstants::NullActionMove)
                    {
                        bool validMove = m_Board->MakeMove(move.StartSquare, actionMove);

                        if (validMove)
                        {
                            m_PreviousMove = move;
                        }
                    }
                }
            }

            // Reset selection
            m_SelectedSquare = GlobalConstants::NullPosition;
            m_MoveType = MoveType::None;
        }
    }
    else
    {
        if (IsCellInBounds(targetSquare, m_Board->GetSize()) && m_Board->IsValidPieceByTurn(targetSquare))
        {
            // Assign selected square
            m_SelectedSquare = targetSquare;
            m_MoveType = moveTypePressed;
        }
        else
        {
            // Reset selection
            m_SelectedSquare = GlobalConstants::NullPosition;
            m_MoveType = MoveType::None;
        }
    }
}

void Application::MoveHandler_MouseReleased(const sf::Event::MouseButtonReleased* mouseReleased)
{
    // Assign move type. 
    // Left click generates legal piece moves
    // Right click generates legal action moves
    MoveType moveTypeRelease = MoveType::None;
    if (mouseReleased->button == sf::Mouse::Button::Left)
        moveTypeRelease = MoveType::Move;
    if (mouseReleased->button == sf::Mouse::Button::Right)
        moveTypeRelease = MoveType::Action;

    sf::Vector2f mousePosition = sf::Vector2f(mouseReleased->position.x, mouseReleased->position.y);
    sf::Vector2i targetSquare = m_Renderer.MouseCellIndex(m_Window.getSize().y, mousePosition);
    if (m_MoveType != MoveType::None && m_MoveType == moveTypeRelease &&
        IsCellInBounds(targetSquare, m_Board->GetSize()))
    {
        // Make move if a square was previously selected
        if (m_SelectedSquare != GlobalConstants::NullPosition && m_SelectedSquare != targetSquare)
        {
            PieceMove move = PieceMove(m_SelectedSquare, targetSquare);
            const auto& selectedPiece = (*m_Board)[move.StartSquare];

            if (selectedPiece.get() != nullptr)
            {
                ActionMove actionMove = selectedPiece->IsLegalMove(move, m_MoveType);
                if (actionMove != GlobalConstants::NullActionMove)
                {
                    bool validMove = m_Board->MakeMove(move.StartSquare, actionMove);

                    if (validMove)
                    {
                        m_PreviousMove = move;
                        // Reset selection
                        m_SelectedSquare = GlobalConstants::NullPosition;
                        m_MoveType = MoveType::None;
                    }
                }
            }
        }
    }
    else
    {
        // Reset selection
        m_SelectedSquare = GlobalConstants::NullPosition;
        m_MoveType = MoveType::None;
    }
}
