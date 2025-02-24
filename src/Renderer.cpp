#include "Renderer.h"

#include <iostream>

void ChessAI::Renderer::DrawBoard(sf::RenderWindow& window, const ChessAI::Board& board) const
{
	// Calculate board size
	sf::Vector2u resolution = window.getSize();
	sf::Vector2f boardPadding = { resolution.x * m_BoardPadding01.x, resolution.y * m_BoardPadding01.y };
	float boardSize = resolution.y - boardPadding.y * 2.f;
	float cellSize = boardSize / 8;
	// Calculate board position (centralised according to screen, drawn from the bottom left)
	float boardPositionX = (resolution.x - boardSize) / 2.f;
	float boardPositionY = resolution.y * m_BoardPadding01.y;

	// Draw board
	{
		sf::RectangleShape blackSquare(sf::Vector2f(cellSize, cellSize));
		sf::RectangleShape whiteSquare(sf::Vector2f(cellSize, cellSize));
		blackSquare.setFillColor(m_ColorDarkSquare);
		whiteSquare.setFillColor(m_ColorWhiteSquare);

		for (int rank = 0; rank < 8; rank++)
		{
			for (int file = 0; file < 8; file++)
			{
				auto& square = (file + rank) % 2 == 0 ? blackSquare : whiteSquare;
				sf::Vector2f position = sf::Vector2f(boardPositionX + file * cellSize, boardPositionY + rank * cellSize);
				square.setPosition(position);
				window.draw(square);
			}
		}
	}

	// Draw pieces
	{
		std::vector<Piece> pieces;
		board.GetBoard(pieces);

		const ChessAI::ResourceManager& rm = ChessAI::ResourceManager::GetInstance();
		sf::RectangleShape pieceRect(sf::Vector2f(128, 128));
		for (Piece piece : pieces)
		{
			const auto& texture = rm.GetPieceTexture(piece.Type);
			int file = piece.Position % 8;
			int rank = (63 - piece.Position) / 8;
			sf::Vector2f position = sf::Vector2f(boardPositionX + file * cellSize, boardPositionY + rank * cellSize);
			pieceRect.setTexture(&texture);
			pieceRect.setPosition(position);
			window.draw(pieceRect);
		}
	}
}
