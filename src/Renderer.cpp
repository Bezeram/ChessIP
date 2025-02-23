#include "Renderer.h"

#include <iostream>

void ChessAI::Renderer::DrawBoard(sf::RenderWindow& window, const Board& board) const
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

	// Draw pieces
	// TODO:
}
