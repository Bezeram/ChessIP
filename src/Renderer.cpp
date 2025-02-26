#include "Renderer.h"



#include <iostream>

ChessAI::Renderer::Renderer(const sf::Vector2u& screenSize)
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

	if (!m_Shader.loadFromMemory(outlineFixFragShader, sf::Shader::Type::Fragment))
		std::cout << "Shader outlineFix could not be loaded.\n";

	// Calculate rendered board properties
	CalculateBoard(screenSize);
}

void ChessAI::Renderer::DrawBoard(sf::RenderWindow& window, const ChessAI::Board& board, int selectedSquare)
{
	CalculateBoard(window.getSize());

	// Draw board (from the top to bottom, left to right)
	{
		sf::RectangleShape blackSquare(sf::Vector2f(m_BoardCellSize, m_BoardCellSize));
		sf::RectangleShape whiteSquare(sf::Vector2f(m_BoardCellSize, m_BoardCellSize));
		blackSquare.setFillColor(m_ColorDarkSquare);
		whiteSquare.setFillColor(m_ColorWhiteSquare);

		for (int rank = 0; rank < 8; rank++)
		{
			for (int file = 0; file < 8; file++)
			{
				auto& square = (file + rank) % 2 == 0 ? whiteSquare : blackSquare;
				sf::Vector2f position = sf::Vector2f(m_BoardPosition.x + file * m_BoardCellSize, m_BoardPosition.y + rank * m_BoardCellSize);

				// Draw the selected square a different color
				if (rank * 8 + file == selectedSquare)
				{
					sf::RectangleShape selectedSquare = whiteSquare;
					selectedSquare.setFillColor(sf::Color(255, 25.5, 25.5));
					selectedSquare.setPosition(position);
					window.draw(selectedSquare);
				}
				else
				{
					square.setPosition(position);
					window.draw(square);
				}
			}
		}
	}

	// Draw pieces (from the top to bottom, left to right)
	// The position of a piece however is counted from the bottom-left, going left to right and bottom-top
	{
		std::vector<Piece> pieces;
		board.GetBoard(pieces);

		const ChessAI::ResourceManager& rm = ChessAI::ResourceManager::GetInstance();
		for (Piece piece : pieces)
		{
			const auto& texture = rm.GetPieceTexture(piece.Type);
			m_Shader.setUniform(std::string("texture"), sf::Shader::CurrentTexture);

			float scale = m_BoardCellSize / texture.getSize().x;
			// Calculate position
			int file = piece.Position % 8;
			int rank = piece.Position / 8;
			sf::Vector2f position = sf::Vector2f(m_BoardPosition.x + file * m_BoardCellSize, m_BoardPosition.y + rank * m_BoardCellSize);

			sf::Sprite piece(texture);
			piece.setPosition(position);
			piece.setScale(sf::Vector2f(scale, scale));
			window.draw(piece, &m_Shader);
		}
	}
}

void ChessAI::Renderer::CalculateBoard(const sf::Vector2u& screenSize)
{
	sf::Vector2u resolution = screenSize;
	sf::Vector2f boardPadding = { resolution.x * m_BoardPadding01.x, resolution.y * m_BoardPadding01.y };
	m_BoardSize = resolution.y - boardPadding.y * 2.f;
	m_BoardCellSize = m_BoardSize / 8.f;
	// Calculate board position (centralised according to screen, drawn from the bottom left)
	float boardPositionX = (resolution.x - m_BoardSize) / 2.f;
	float boardPositionY = resolution.y * m_BoardPadding01.y;
	m_BoardPosition = sf::Vector2f(boardPositionX, boardPositionY);
}

sf::Vector2f ChessAI::Renderer::GetBoardPosition() const
{
	return m_BoardPosition;
}

float ChessAI::Renderer::GetBoardSize() const
{
	return m_BoardSize;
}

float ChessAI::Renderer::GetBoardCellSize() const
{
	return m_BoardCellSize;
}

bool ChessAI::Renderer::IsMouseOnBoard(const sf::Vector2f& mousePosition) const
{
	return (mousePosition.x >= m_BoardPosition.x && mousePosition.x <= m_BoardPosition.x + m_BoardSize &&
		mousePosition.y >= m_BoardPosition.y && mousePosition.y <= m_BoardPosition.y + m_BoardSize);
}

sf::Vector2i ChessAI::Renderer::MouseCellIndex(const sf::Vector2f& mousePosition) const
{
	return sf::Vector2i(
		int((mousePosition.x - m_BoardPosition.x) / m_BoardCellSize),
		int((mousePosition.y - m_BoardPosition.y) / m_BoardCellSize));
}
