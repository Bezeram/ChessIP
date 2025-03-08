#include "Renderer.h"

#include <iostream>

using namespace ChessIP;

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

void Renderer::DrawBoard(sf::RenderWindow& window, const Board& board, int selectedSquare, const Move& previousMove)
{
	CalculateBoard(window.getSize(), board.GetSize());
	auto ifHighlightDraw = [&](int position, sf::RectangleShape tile)
		{
			// Highlight color is additive over grid
			if (position == selectedSquare)
				tile.setFillColor(m_ColorSelectSquare);
			else if (position == previousMove.StartSquare || position == previousMove.TargetSquare)
				tile.setFillColor(m_ColorPreviousMove);
			
			window.draw(tile);
		};

	{
		sf::RectangleShape tile(sf::Vector2f(m_BoardCellSize, m_BoardCellSize));

		int boardGridSize = board.GetSize();
		for (int rank = 0; rank < boardGridSize; rank++)
		{
			for (int file = 0; file < boardGridSize; file++)
			{
				int position = rank * boardGridSize + file;
				sf::Color gridColor = (rank + file) % 2 == 0 ? m_ColorWhiteSquare : m_ColorDarkSquare;
				sf::Vector2f position2D = sf::Vector2f(m_BoardPosition.x + file * m_BoardCellSize, m_BoardPosition.y + rank * m_BoardCellSize);
				// Draw grid tile
				tile.setFillColor(gridColor);
				tile.setPosition(position2D);
				window.draw(tile);

				// If special tile, draw additive highlights
				ifHighlightDraw(position, tile);
			}
		}
	}
	
	// Draw pieces (from the top to bottom, left to right)
	// The position of a piece however is counted from the bottom-left, going left to right and bottom-top
	{
		std::vector<Piece> pieces;
		board.GetBoard(pieces);

		const ResourceManager& rm = ResourceManager::GetInstance();
		for (Piece piece : pieces)
		{
			const auto& texture = rm.GetPieceTexture(piece.Type);
			m_PieceShader.setUniform(std::string("texture"), sf::Shader::CurrentTexture);

			float scale = m_BoardCellSize / texture.getSize().x;
			// Calculate position
			int boardGridSize = board.GetSize();
			int file = piece.Position % boardGridSize;
			int rank = piece.Position / boardGridSize;
			sf::Vector2f position = sf::Vector2f(m_BoardPosition.x + file * m_BoardCellSize, m_BoardPosition.y + rank * m_BoardCellSize);

			sf::Sprite piece(texture);
			piece.setPosition(position);
			piece.setScale(sf::Vector2f(scale, scale));
			window.draw(piece, &m_PieceShader);
		}
	}
}

void Renderer::CalculateBoard(const sf::Vector2u& screenSize, int boardGridSize)
{
	sf::Vector2u resolution = screenSize;
	sf::Vector2f boardPadding = { resolution.x * m_BoardPadding01.x, resolution.y * m_BoardPadding01.y };
	m_BoardLength = resolution.y - boardPadding.y * 2.f;
	m_BoardCellSize = m_BoardLength / (float)boardGridSize;
	// Calculate board position (centralised according to screen, drawn from the bottom left)
	float boardPositionX = (resolution.x - m_BoardLength) / 2.f;
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
