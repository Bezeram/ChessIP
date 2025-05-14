#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Board.h"
#include "ResourceManager.h"

class Renderer
{
public:
	Renderer(const sf::Vector2u& screenSize, int boardGridSize);

	/*
		Board is drawn from white's perspective, from the top left square, left to right;
	*/
	void DrawBoard(sf::RenderWindow& window, const Board& board, PiecePosition selectedSquare, const PieceMove& previousMove);
	void CalculateBoard(const sf::Vector2u& screenSize, int boardTileSize);

	sf::Vector2f GetBoardPosition() const;
	float GetBoardSize() const;
	float GetBoardCellSize() const;

	bool IsMouseOnBoard(const sf::Vector2f& mousePosition) const;
	sf::Vector2i MouseCellIndex(const sf::Vector2f& mousePosition) const;
private:
	sf::Shader m_PieceShader;

	// Expressed as a percentage of the viewport size
	sf::Vector2f m_BoardPadding01 = { 0.05f, 0.05f };
	sf::Vector2f m_BoardPosition = { 0.f, 0.f };
	float m_BoardLength;
	float m_BoardCellSize;

	sf::Color m_ColorDarkSquare = { 89, 56, 0 };
	sf::Color m_ColorWhiteSquare = { 255, 223, 170 };
	sf::Color m_ColorSelectSquare = { 255, 184, 0, 125 };
	sf::Color m_ColorPreviousMove = { 255, 209, 0, 125 };

	sf::Color m_ColorLegalMove = { 255, 30, 30 };
};
