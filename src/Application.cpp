#include "Application.h"

Application::Application(WindowSettings settings)
    : m_Window(sf::RenderWindow(
        (settings.Resolution.x == 0 || settings.Resolution.y == 0) ? sf::VideoMode(sf::Vector2u(1920, 1080)) : sf::VideoMode(settings.Resolution),
        "Chess9", sf::Style::Default, settings.State)
    )
    , m_Inventory(m_DefaultDeck)
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
    sf::Clock clock;

    while (m_IsRunning)
    {
        sf::Time deltaTime = clock.restart();
        EventHandler(); // actualizează și mouse-ul

        m_Window.clear();

        if (m_IsInMenu)
        {
            // === MENU MODE ===
            m_Renderer.DrawBackground(m_Window);
            m_Renderer.DrawMenu(m_Window);
        }
        else
        {
            // === GAME MODE ===
            /*if (m_Board->GetWhiteKingPosition() == Constants::NullPosition ||
                m_Board->GetBlackKingPosition() == Constants::NullPosition)
            {
                m_Board->ResetBoard(m_Board);
            }*/
            m_Renderer.DrawBackground(m_Window);
            m_Renderer.DrawBoard(m_Window, *m_Board, m_SelectedSquare, m_MoveType, m_PreviousMove, deltaTime);
            m_Renderer.DrawInventory(m_Window, m_Inventory, m_SelectedInventorySlot, deltaTime);
            m_Renderer.DrawInfo(m_Window);

            // Draw the resource bars associated with the current player's move
            Board::ResourcesType activePlayerResources = m_Board->GetActivePlayerResources();
            m_Renderer.DrawResourceBars(m_Window, activePlayerResources.flux, activePlayerResources.gold);
        }

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
            m_Window.setView(m_Viewport);
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
                    Global::AdjustableK -= 0.5f;
                else
                    Global::AdjustableK += 0.5f;
                std::cout << "K: " << Global::AdjustableK << std::endl;
            }

            if (keyPressed->scancode == sf::Keyboard::Scan::U)
            {
                if (m_SelectedSquare != Constants::NullPosition)
				{
					const auto& selectedPiece = (*m_Board)[m_SelectedSquare];
                    int cost = Constants::PieceTypeToGoldCost.at(selectedPiece->GetPieceType());
					if (selectedPiece.get() != nullptr && m_Board->GetGold() >= cost)
					{
						selectedPiece->Upgrade();
                        m_Board->IncreaseGold(-cost);
					}
				}
            }
        }
        else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            Global::MouseLeftPressed = true;

            if (m_IsInMenu)
            {
                sf::Vector2f mousePos = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window));
                sf::FloatRect playBounds = m_Renderer.GetPlayButtonBounds();

                if (playBounds.contains(mousePos))
                {
                    m_IsInMenu = false;
                    return;
                }
            }
            else
            {
                MoveHandler_MousePressed(mousePressed);
                InventoryHandler_MousePressed(mousePressed);
                PiecePlacerHandler_MousePressed(mousePressed);
            }
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
        m_SelectedSquare != Constants::NullPosition)
    {
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
                    if (actionMove != Constants::NullActionMove)
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
            m_SelectedSquare = Constants::NullPosition;
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
            m_SelectedSquare = Constants::NullPosition;
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
        // If the move is an action, allow for the target square to be selected again
        if (m_SelectedSquare != Constants::NullPosition && 
            (m_SelectedSquare != targetSquare || m_MoveType == MoveType::Action))
        {
            PieceMove move = PieceMove(m_SelectedSquare, targetSquare);
            const auto& selectedPiece = (*m_Board)[move.StartSquare];

            if (selectedPiece.get() != nullptr)
            {
                ActionMove actionMove = selectedPiece->IsLegalMove(move, m_MoveType);
                if (actionMove != Constants::NullActionMove)
                {
                    bool validMove = m_Board->MakeMove(move.StartSquare, actionMove);

                    if (validMove)
                    {
                        m_PreviousMove = move;
                        // Reset selection
                        m_SelectedSquare = Constants::NullPosition;
                        m_MoveType = MoveType::None;
                    }
                }
            }
        }
    }
    else
    {
        // Reset selection
        m_SelectedSquare = Constants::NullPosition;
        m_MoveType = MoveType::None;
    }
}

void Application::InventoryHandler_MousePressed(const sf::Event::MouseButtonPressed* buttonPressed)
{
    // Get position
    sf::Vector2f mousePosition = sf::Vector2f(buttonPressed->position.x, buttonPressed->position.y);

    // If it's on the board immediately return
    sf::Vector2i targetSquare = m_Renderer.MouseCellIndex(m_Window.getSize().y, mousePosition);
    if (IsCellInBounds(targetSquare, m_Board->GetSize()))
        return;

    sf::Vector2f inventoryPosition = m_Renderer.CalculateInventoryPosition();
    sf::Vector2f inventoryScale = m_Renderer.CalculateInventoryScale();
    // Constant texture pixel sizes
    sf::Vector2f topLeftSlotPosition = Inventory::s_TopLeftSlotPosition.componentWiseMul(inventoryScale);
    sf::Vector2f inventorySize = Inventory::s_InventorySize.componentWiseMul(inventoryScale);

    // Remove inventory outside padding
    sf::Vector2f normalizedPosition = mousePosition - inventoryPosition - topLeftSlotPosition;
    // divide by inventory length without outside padding
    // result is mouse 
    sf::Vector2f internalSlotSize = inventorySize - (topLeftSlotPosition * 2.f);

    if (normalizedPosition.x >= 0 && normalizedPosition.x <= internalSlotSize.x &&
        normalizedPosition.y >= 0 && normalizedPosition.y <= internalSlotSize.y)
    {
        // Mouse position is inside slot area rectangle
        sf::Vector2f slotGap = Inventory::s_SlotGap.componentWiseMul(inventoryScale);
        sf::Vector2f slotSize = Inventory::s_SlotSize.componentWiseMul(inventoryScale);

        // Get rid of the spaces between slots
        sf::Vector2f fullSlotSize = slotGap + slotSize;

        // Add imaginary slot gap so division makes sense
        sf::Vector2f fullSlotIndex = (normalizedPosition).componentWiseDiv(fullSlotSize);

        sf::Vector2f fracPartSlotIndex = { 
            fullSlotIndex.x - std::floor(fullSlotIndex.x), 
            fullSlotIndex.y - std::floor(fullSlotIndex.y) };

        // If mouse is in the "gap" part of the fullslot segment then it doesn't belong to a slot
        if (fracPartSlotIndex.x > slotSize.x / fullSlotSize.x ||
            fracPartSlotIndex.y > slotSize.y / fullSlotSize.y)
        {
            // Reset selection
            m_SelectedInventorySlot = Constants::NullPosition;
            return;
        }

        // Assign inventory slot
        m_SelectedInventorySlot = sf::Vector2i(fullSlotIndex);
        return;
    }

    // Reset selection
    m_SelectedInventorySlot = Constants::NullPosition;
}

void Application::PiecePlacerHandler_MousePressed(const sf::Event::MouseButtonPressed* buttonPressed)
{
    if (m_SelectedInventorySlot == Constants::NullPosition || !m_Board->IsWhitesMove())
        return;
    
    int indexPiece = m_SelectedInventorySlot.y * 4 + m_SelectedInventorySlot.x;
    PieceType pieceType = m_Inventory.GetDeckPiece(indexPiece);

    sf::Vector2f mousePosition = sf::Vector2f(buttonPressed->position);
    sf::Vector2i targetSquare = m_Renderer.MouseCellIndex(m_Window.getSize().y, mousePosition);
    if (IsCellInBounds(targetSquare, m_Board->GetSize()))
    {
        auto& selectedPiece = (*m_Board)[targetSquare];

        if (selectedPiece.get() == nullptr && m_Board->GetFlux() >= Constants::PieceTypeToFluxCost.at(pieceType))
        {
            // Place a new piece at the specified location
            // drain flux
            selectedPiece.reset(PieceFactory::Create(pieceType, GetPieceColor(pieceType), m_Board));

            m_Board->IncreaseFlux(-Constants::PieceTypeToFluxCost.at(pieceType));
        }
    }
}
