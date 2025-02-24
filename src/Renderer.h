#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Board.h"
#include "ResourceManager.h"

namespace ChessAI
{
	class Renderer
	{
	public:
		Renderer() = default;

		/*
			Board is drawn from white's perspective, from the bottom left square, left to right;
		*/
		void DrawBoard(sf::RenderWindow& window, const Board& board) const;
	private:

		// Expressed as a percentage of the viewport size
		sf::Vector2f m_BoardPadding01 = { 0.1f, 0.1f };

		sf::Color m_ColorDarkSquare = { 89, 56, 0 };
		sf::Color m_ColorWhiteSquare = { 255, 223, 170 };
	};
}
