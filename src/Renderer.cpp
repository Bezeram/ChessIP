#include "Renderer.h"

#include <iostream>

Renderer::Renderer(const sf::Vector2u& screenSize, int boardTileSize)
{
	// Load shader for pieces
	constexpr std::string_view outlineFixFragShader = R"(
		uniform sampler2D texture;

		void main()
		{
			vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    
			// Discard almost-transparent pixels to avoid white outlines
			if (color.a < 0.7)
				discard;
			else if (color.a < 1.0)
				color = vec4(0, 0, 0, 1);

			gl_FragColor = color;
		}
	)";

	if (!m_PieceShader.loadFromMemory(outlineFixFragShader, sf::Shader::Type::Fragment))
		std::cout << "Shader outlineFix could not be loaded.\n";

	// Calculate rendered board properties
	CalculateBoardProperties(screenSize, boardTileSize);
}

void Renderer::DrawBoard(sf::RenderWindow& window, const Board& board, PiecePosition selectedPiecePosition, MoveType moveType, const PieceMove& previousMove)
{
	CalculateBoardProperties(window.getSize(), board.GetSize());

	// Helper function for drawing a board tile
	sf::RectangleShape tile(sf::Vector2f(m_BoardCellSize, m_BoardCellSize));
	auto drawSquare = [&](PiecePosition position, sf::Color color)
		{
			sf::Vector2f drawPosition = CalculateTilePosition(window.getSize().y, position);

			tile.setFillColor(color);
			tile.setPosition(drawPosition);
			window.draw(tile);
		};

	/// Get legal moves
	std::vector<ActionMove> legalMoves;
	if (selectedPiecePosition != GlobalConstants::NullPosition)
	{
		const auto& selectedPiece = board[selectedPiecePosition];
		if (selectedPiece.get() != nullptr)
			selectedPiece->GetLegalMoves(selectedPiecePosition, legalMoves);
	}

	/// Draw grid board
	for (int rank = 0; rank < board.GetSize(); rank++)
	{
		for (int file = 0; file < board.GetSize(); file++)
		{
			PiecePosition gridPosition = PiecePosition(file, rank);
			sf::Color gridColor = (rank + file) % 2 == 0 ? m_ColorWhiteSquare : m_ColorDarkSquare;

			drawSquare(gridPosition, gridColor);
		}
	}

	/// Draw legal moves, previous move and highlight selected square
	// 
	// Legal moves must be drawn on top of any other highlight
	bool skipPreviousMoveRender[2] = { false, false };
	bool skipSelectHighlight = false;
	// Highlight legal moves and selected square
	if (selectedPiecePosition != GlobalConstants::NullPosition)
	{
		// Legal moves
		for (const auto& move : legalMoves)
		{
			// Only consider the specified move type
			if (move.MoveType == moveType)
			{
				sf::Color color = (moveType == MoveType::Move) ? m_ColorLegalMove : m_ColorLegalAction;
				drawSquare(move.TargetSquare, color);

				// Register possible collisions with the previous move..
				if (move.TargetSquare == previousMove.StartSquare)
					skipPreviousMoveRender[0] = true;
				if (move.TargetSquare == previousMove.TargetSquare)
					skipPreviousMoveRender[1] = true;
				// ..and with the select position
				if (move.TargetSquare == selectedPiecePosition)
					skipSelectHighlight = true;
			}
		}

		// Selected piece square
		if (!skipSelectHighlight)
			drawSquare(selectedPiecePosition, m_ColorSelectSquare);
	}
	// Draw previous move
	if (previousMove.StartSquare != GlobalConstants::NullPosition && previousMove.TargetSquare != GlobalConstants::NullPosition)
	{
		// Highlight previous move if there were no collisions
		if (!skipPreviousMoveRender[0])
			drawSquare(previousMove.StartSquare, m_ColorPreviousMove);
		if (!skipPreviousMoveRender[1])
			drawSquare(previousMove.TargetSquare, m_ColorPreviousMove);
	}
	
	/// Draw pieces (from the top to bottom, left to right)
	// 
	// The position of a piece is counted from the bottom-left, going left to right and bottom-top
	{
		const ResourceManager& rm = ResourceManager::GetInstance();
		for (int rank = 0; rank < board.GetSize(); rank++)
		{
			for (int file = 0; file < board.GetSize(); file++)
			{
				PiecePosition piecePosition = PiecePosition(file, rank);
				const auto& piece = board[piecePosition];
				if (piece.get() == nullptr)
					continue;

				// Set texture and shader
				const auto& texture = rm.GetPieceTexture(piece->GetPieceType());
				m_PieceShader.setUniform(std::string("texture"), sf::Shader::CurrentTexture);

				sf::Sprite sprite(texture);
				// Set sprite position and scale on board tile
				sf::Vector2f drawPosition = CalculateTilePosition(window.getSize().y, piecePosition);
				float scale = m_BoardCellSize / texture.getSize().x;
				sprite.setPosition(drawPosition);
				sprite.setScale(sf::Vector2f(scale, scale));

				// Sprite is finally rendered using its own function
				bool isSelectedPiece = piecePosition == selectedPiecePosition;
				piece->Render(sprite, window, m_PieceShader, isSelectedPiece);
			}
		}
	}
}

void Renderer::CalculateBoardProperties(const sf::Vector2u& screenSize, int boardGridSize)
{
	sf::Vector2u resolution = screenSize;
	sf::Vector2f boardPadding = { resolution.x * m_BoardPadding01.x, resolution.y * m_BoardPadding01.y };
	m_BoardLength = resolution.y - boardPadding.y * 2.f;
	m_BoardCellSize = m_BoardLength / (float)boardGridSize;
	// Calculate board position (centralised according to screen, drawn from the bottom left)
	float boardPositionX = (resolution.x - m_BoardLength) / 8.f;
	float boardPositionY = resolution.y * m_BoardPadding01.y;
	m_BoardPosition = sf::Vector2f(boardPositionX, boardPositionY);
}

void Renderer::DrawHUD(const sf::Vector2u& screenSize, int boardTileSize)
{
	// TODO: (Lungu)
}

sf::Vector2f Renderer::GetBoardPosition() const
{
	return m_BoardPosition;
}

float Renderer::GetBoardSize() const
{
	return m_BoardLength;
}

float Renderer::GetBoardCellSize() const
{
	return m_BoardCellSize;
}

sf::Vector2f Renderer::CalculateTilePosition(uint32_t windowHeight, PiecePosition position)
{
	int file = position.x;
	int rank = position.y;
	return sf::Vector2f(m_BoardPosition.x + file * m_BoardCellSize,
		windowHeight - m_BoardPosition.y - (rank + 1) * m_BoardCellSize);
}

sf::Vector2i Renderer::MouseCellIndex(int windowHeight, const sf::Vector2f& mousePosition) const
{
	return sf::Vector2i(
		int((std::floor((mousePosition.x - m_BoardPosition.x) / m_BoardCellSize))),
		int((windowHeight - (mousePosition.y - m_BoardPosition.y)) / m_BoardCellSize) - 1);
}
