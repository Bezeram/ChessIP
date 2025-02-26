#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Board.h"
#include "ResourceManager.h"

namespace ChessIP
{
	class Renderer
	{
	public:
		Renderer(const sf::Vector2u& screenSize);

		/*
			Board is drawn from white's perspective, from the bottom left square, left to right;
		*/
		void DrawBoard(sf::RenderWindow& window, const Board& board, int selectedSquare);
		void CalculateBoard(const sf::Vector2u& screenSize);

		sf::Vector2f GetBoardPosition() const;
		float GetBoardSize() const;
		float GetBoardCellSize() const;

		bool IsMouseOnBoard(const sf::Vector2f& mousePosition) const;
		sf::Vector2i MouseCellIndex(const sf::Vector2f& mousePosition) const;
	private:
		sf::Shader m_Shader;

		// Expressed as a percentage of the viewport size
		sf::Vector2f m_BoardPadding01 = { 0.05f, 0.05f };
		sf::Vector2f m_BoardPosition = { 0.f, 0.f };
		float m_BoardSize;
		float m_BoardCellSize;

		sf::Color m_ColorDarkSquare = { 89, 56, 0 };
		sf::Color m_ColorWhiteSquare = { 255, 223, 170 };
	};
}
