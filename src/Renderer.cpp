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
	CalculateBoard(screenSize, boardTileSize);
}

void Renderer::DrawBoard(sf::RenderWindow& window, const Board& board, PiecePosition selectedPiecePosition, const PieceMove& previousMove)
{
	CalculateBoard(window.getSize(), board.GetSize());
	// Function for drawing a square
	auto drawSquare = [&](PiecePosition position, sf::Color color)
		{
			sf::RectangleShape tile(sf::Vector2f(m_BoardCellSize, m_BoardCellSize));
			sf::Vector2f drawPosition = m_BoardPosition + sf::Vector2f(position.y, position.x) * m_BoardCellSize;

			tile.setFillColor(color);
			tile.setPosition(drawPosition);
			window.draw(tile);
		};

	// Check for valid moves to highlight over the board
	std::vector<ActionMove> legalMoves;
	if (selectedPiecePosition != GlobalConstants::NullPosition)
	{
		const auto& selectedPiece = board[selectedPiecePosition];
		if (selectedPiece.get() != nullptr)
			selectedPiece->GetLegalMoves(board.GetBoard(), selectedPiecePosition, legalMoves);
	}

	// Draw grid board
	for (int rank = 0; rank < board.GetSize(); rank++)
	{
		for (int file = 0; file < board.GetSize(); file++)
		{
			PiecePosition gridPosition = PiecePosition(rank, file);
			sf::Color gridColor = (rank + file) % 2 == 0 ? m_ColorWhiteSquare : m_ColorDarkSquare;

			drawSquare(gridPosition, gridColor);
		}
	}

	// Draw legal moves and previous move highlight
	if (selectedPiecePosition != GlobalConstants::NullPosition)
	{
		// Legal moves
		for (const auto& move : legalMoves)
		{
			drawSquare(move.TargetSquare, m_ColorLegalMove);
		}

		// Selected piece square
		drawSquare(selectedPiecePosition, m_ColorSelectSquare);
	}
	if (previousMove.StartSquare != GlobalConstants::NullPosition && previousMove.TargetSquare != GlobalConstants::NullPosition)
	{
		drawSquare(previousMove.StartSquare, m_ColorPreviousMove);
		drawSquare(previousMove.TargetSquare, m_ColorPreviousMove);
	}
	
	// Draw pieces (from the top to bottom, left to right)
	// The position of a piece is counted from the bottom-left, going left to right and bottom-top
	{
		const ResourceManager& rm = ResourceManager::GetInstance();
		for (int rank = 0; rank < board.GetSize(); rank++)
		{
			for (int file = 0; file < board.GetSize(); file++)
			{
				PiecePosition position = PiecePosition(rank, file);
				const auto& piece = board[position];
				if (piece.get() == nullptr)
					continue;

				std::cout << "[Rank, File, PieceType]: " << rank << ", " << file << ", " 
					<< Textures::PieceTypeToString.at(piece->GetPieceType()) << std::endl;

				// Set texture and shader
				const auto& texture = rm.GetPieceTexture(piece->GetPieceType());
				m_PieceShader.setUniform(std::string("texture"), sf::Shader::CurrentTexture);

				// Calculate position
				sf::Sprite sprite(texture);
				sf::Vector2f drawPosition;

				// Setup position
				if (position == selectedPiecePosition && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					// Piece is dragged with mouse
					sf::Vector2i mousePosition = sf::Mouse::getPosition();
					drawPosition = sf::Vector2f(mousePosition.x, mousePosition.y);

					sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.f, texture.getSize().y / 2.f));
				}
				else
					drawPosition = sf::Vector2f(m_BoardPosition.x + file * m_BoardCellSize, m_BoardPosition.y + rank * m_BoardCellSize);
				float scale = m_BoardCellSize / texture.getSize().x;

				sprite.setPosition(drawPosition);
				sprite.setScale(sf::Vector2f(scale, scale));
				window.draw(sprite, &m_PieceShader);
			}
		}
	}

	// Draw resources bars
	// TODO: Draw resources bars
	{
		
	}
}

void Renderer::CalculateBoard(const sf::Vector2u& screenSize, int boardGridSize)
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

bool Renderer::IsMouseOnBoard(const sf::Vector2f& mousePosition) const
{
	return (mousePosition.x >= m_BoardPosition.x && mousePosition.x <= m_BoardPosition.x + m_BoardLength &&
		mousePosition.y >= m_BoardPosition.y && mousePosition.y <= m_BoardPosition.y + m_BoardLength);
}

sf::Vector2i Renderer::MouseCellIndex(const sf::Vector2f& mousePosition) const
{
	return sf::Vector2i(
		int((mousePosition.x - m_BoardPosition.x) / m_BoardCellSize),
		int((mousePosition.y - m_BoardPosition.y) / m_BoardCellSize));
}
